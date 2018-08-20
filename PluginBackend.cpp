#define PLUGINBACKEND_CPP
#include "PluginBackend.h"
#define PluginBackend_CPP
#include "PluginBackend.h"
#include <YbCore/controller>
#include <YbCore/coretool>
#include <YbCore/mesh_proc>
#include <glm/glm.hpp>
#include <QStandardPaths>
#include <glm/gtc/matrix_transform.hpp>
#include "./algorithm/MeshProcessing.h"

#include "SliceRender.h"
PluginBackend::PluginBackend(IPluginBackend* parent)
{
}

void PluginBackend::workMode(QString mode) {
    if(mode == "scan")
        render_s->changeRender(std::bind(&SliceRender::scanLineAnimation, render_s, std::placeholders::_1));
    else if(mode == "pick")
        render_s->changeRender(std::bind(&SliceRender::drawModelWithSlice, render_s, std::placeholders::_1));
    con<RenderCtrl>().update();
}

void PluginBackend::construction() {
#if  defined(Q_OS_OSX)
    std::string prefix = "../PlugIns/";
#elif defined(Q_OS_WIN)
    std::string prefix = "./bin/";
#endif
    QString shader_prefix = QString::fromStdString(prefix)+"Measurement/shaders/";
    RenderScript([=](QTime& t) {
        con<ShaderCtrl>().addShaderProgram("base", shaderConfig{ V(shader_prefix+"indices"),F(shader_prefix+"indices") });
    });
    render_s = new("base") SliceRender();
    this->importMesh(prefix+"GLViewer/mesh/body2.obj","scanbody");
}

void PluginBackend::destruction() {

}

void PluginBackend::slice(float dis) {
    PickableMesh* mesh = con<MeshCtrl>().mesh("scanbody");
    auto interval = YbCore::getSliceInterval(mesh, norm, dis, gap);
    std::vector<std::array<glm::vec3,2>> slice_res;
    for(auto it = interval[0]; it != interval[1]; it++) {
        if(YbCore::isFaceInersected(mesh, *it, norm, dis)) {
            slice_res.emplace_back(YbCore::getFaceIntersection(mesh, *it, norm, dis));
        }
    }

    auto view = con<ViewCtrl>().view();
    auto mvp  = view->matrixVP()*view->model()* glm::rotate(glm::mat4(),-3.1415926f*0.5f,glm::vec3(1,0,0))*mesh->model;
    float w = canvas->width();
    float h = canvas->height();
    std::vector<float> girths;

    contours.clear();
    for(auto& intervals: plugin::sortContours(slice_res)) {
        contours.emplace_back(std::vector<QPoint>());
        girths.emplace_back(0);
        for(auto it = intervals[0]; it != intervals[1]; it++) {
            glm::vec3 p = (glm::vec3(mvp*glm::vec4(mesh->v[(*it)[0][0]]+(*it)[0][2]*(mesh->v[(*it)[0][1]] - mesh->v[(*it)[0][0]]),1.0))+1.0f)*0.5f;
            std::prev(contours.end())->emplace_back(QPoint(p[0]*w,(1-p[1])*h));
            *std::prev(girths.end()) += glm::length(mesh->v[(*it)[0][0]]+(*it)[0][2]*(mesh->v[(*it)[0][1]] - mesh->v[(*it)[0][0]])-
                                                    mesh->v[(*it)[1][0]]-(*it)[1][2]*(mesh->v[(*it)[1][1]] - mesh->v[(*it)[1][0]]));
        }
    }

    emit sliceUpdate(QString::number(*std::max_element(girths.begin(),girths.end()),'f',2));

//    plugin::writeSlices(mesh, slice_res, QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/slice.obj");

    canvas->update();
}

void PluginBackend::slice(int x, int y) {
    int f_id = con<CentralCtrl>().pickTool->getSingleFacePick(x,y);
    if(f_id == -1) {
         canvas->update();
         return ;
    }
    PickableMesh* mesh = con<MeshCtrl>().mesh("scanbody");
    render_s->dis = glm::dot(norm, (mesh->v[mesh->f[f_id][0]]+mesh->v[mesh->f[f_id][1]]+mesh->v[mesh->f[f_id][2]])/3.0f);
    slice(render_s->dis);
    con<RenderCtrl>().update();
}

bool PluginBackend::importMesh(std::string url,std::string name){
    PickableMesh* mesh =  YbCore::IO::readObj(url);
    YbCore::calculateNorm(mesh);
    YbCore::centerlized(mesh);
    norm = YbCore::pca_analysic(mesh,2);  glm::vec3(0,0,1);//
    YbCore::sortByVector(mesh, norm);
    con<MeshCtrl>().addMesh(name,mesh); //bunny FullBodyScan 20180205142827.cie

    RenderScript([=](QTime&) {
        mesh->createBufferScript();
        mesh->syncVertexBuffersDataScript();
        mesh->syncFacesBuffersDataScript();
    });

    render_s->bounding_z[0] = glm::dot(norm,mesh->v[(*mesh->f.begin())[0]]);
    render_s->bounding_z[1] = glm::dot(norm,mesh->v[(*std::prev(mesh->f.end()))[0]]);

    float model_height = glm::dot(norm,mesh->v[(*mesh->f.begin())[0]]-mesh->v[(*std::prev(mesh->f.end()))[0]]);
    emit heightUpdate(QString::number(model_height,'f',2));
    for(int i=0; i < mesh->f.size(); i += 10) {
        gap = std::max(gap, abs(glm::dot(norm,mesh->v[mesh->f[i][0]]-mesh->v[mesh->f[i][1]]))*20);
    }

    QObject::connect(mesh,&PickableMesh::Selected, mesh, [=](PickableMesh* mesh){
        auto select_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/selections.obj";
        plugin::writePartialMesh(mesh, con<CentralCtrl>().selectTool->getSelectedFace(), select_file);
        auto border_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/borders.obj";
        auto border_face = plugin::extractMeshBorder(mesh, con<CentralCtrl>().selectTool->getSelectedFace());
//        plugin::writePartialMesh(mesh, border_face, border_file);
    });

    return true;
}

void PluginBackend::setSliceCanvas(QQuickPaintedItem* item) {
    canvas = (SlicesCanvas*)item;
}

#include <unsupported/Eigen/FFT>

void PluginBackend::test(std::vector<std::complex<float> >&  timevec) {
    static  int freq = 15;
    Eigen::FFT<float> fft;

    std::vector<std::complex<float> > freqvec;

    fft.fwd( freqvec,timevec);

    if(freqvec.size() > freq * 5)
        std::fill(freqvec.begin()+std::min((int)timevec.size(), freq),  freqvec.end()-std::min((int)timevec.size(), freq), std::complex<float>(0,0));

    fft.inv( timevec,freqvec);
}

template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}

//#include "moc_PluginBackend.cpp"
