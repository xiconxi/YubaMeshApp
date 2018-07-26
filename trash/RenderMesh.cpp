#include "RenderMesh.h"
#include "../controller/RenderController.h"
#include "../controller/ShaderController.h"
#include "../controller/ViewController.h"
#include "meshCodes.h"
#include "meshProcessing.h"

#include <fstream>
#include <functional>
#include <easylog/easylogging++.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

glFormatMesh* RenderMesh::Mesh(){
    return &mesh;
}

RenderMesh::RenderMesh(std::string filename)
{
    if(filename.substr(filename.length()-4,4).compare(".off") == 0)
        readOff(filename, &mesh);
    else if(filename.substr(filename.length()-4,4).compare(".obj") == 0)
        readObj(filename, &mesh);
    else if(filename.substr(filename.length()-4,4).compare(".cie") == 0) {
        readCIE(filename, &mesh);
        this->setTexture(filename.replace(filename.length()-4,4,".jpg"));
    }
    calculateNorm(&mesh);
    centerlized(&mesh);
    initialize();
}

RenderMesh::RenderMesh(glFormatMesh &&mesh) {
    this->mesh = std::move(mesh);
    initialize();
}

void RenderMesh::initialize() {
    this->initGLVertexArrays();
    this->syncGLVertexArrays();
    if(this->texture_qimg.isNull() == false)
        this->initGLTexture();
//    LOG(INFO) << mesh.v.size() << ' ' << mesh.f.size() << std::endl;
}

void RenderMesh::initGLVertexArrays() {

}

void RenderMesh::initGLTexture() {
    if(texture != nullptr)
        delete texture;
    texture = new QOpenGLTexture(texture_qimg);
    texture->bind();
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
    texture->release();
}

void RenderMesh::syncGLVertexArrays(){
//    glBindVertexArray(mesh.vao);
//    glBindBuffer(GL_ARRAY_BUFFER, mesh.v_buffer);
//    glBufferData(GL_ARRAY_BUFFER, mesh.v.size()*sizeof(mesh.v[0]),mesh.v.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh.v[0]), (GLvoid*)0);
//    glEnableVertexAttribArray(0);

//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh.v[0]), (GLvoid*)sizeof(glm::vec3));
//    glEnableVertexAttribArray(1);

//    glBindBuffer(GL_ARRAY_BUFFER, mesh.tex_buffer);
//    glBufferData(GL_ARRAY_BUFFER, mesh.vts.size()*sizeof(mesh.vts[0]),mesh.vts.data(), GL_STATIC_DRAW);

//    for(auto i:{0,1,2,3}) {
//        glVertexAttribPointer(2+i, 3, GL_FLOAT, GL_FALSE, sizeof(mesh.vts[0]), (GLvoid*)(sizeof(mesh.vts[0][0])*i));
//        glEnableVertexAttribArray(2+i);
//    }

//    glBindVertexArray(0);
}

void RenderMesh::indicesDraw(int start, int count,GLint element_buffer) {
    if(count == -1) count = mesh.f.size();
    //    if(texture) texture->bind();
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer==-1?mesh.ibo:element_buffer);
    glDrawElements(GL_TRIANGLES, count*3, GL_UNSIGNED_INT, (GLvoid*)(start*3*sizeof(unsigned int)));
    glBindVertexArray(0);
    //   if(texture) texture->release();
}


std::pair<int,int> RenderMesh::edge(int x, int y) {
    return x > y ? std::make_pair(x,y):std::make_pair(y,x);
}


void RenderMesh::setTexture(std::string file) {
    texture_qimg = QImage(QString::fromStdString(file));
}


/*
typedef std::map<std::pair<int,int>, int, std::function<bool(const std::pair<int,int>&, const std::pair<int,int>&)>> pairMap;
auto _map = pairMap( [](const std::pair<int,int>& e1, const std::pair<int,int>& e2){
                return (e1.first==e2.first)?(e1.second>e2.second):(e1.first>e2.first);
});


void meshObject::_subdivision() {
    auto _edge = std::map<std::pair<int,int>, int, std::function<bool(const std::pair<int,int>&, const std::pair<int,int>&)>>{
                                                                                                                             [](const std::pair<int,int>& e1, const std::pair<int,int>& e2){
        return (e1.first==e2.first)?(e1.second>e2.second):(e1.first>e2.first);
    }
};
    for(auto& e:mesh.f){
        if(_edge.find(this->edge(e[0],e[1])) == _edge.end()){
            _edge[this->edge(e[0],e[1])] = mesh.v.size();
            mesh.v.push_back(VertItem((mesh.v[e[0]].mv+mesh.v[e[1]].mv)/2.0f));
        }
        if(_edge.end() == _edge.find(this->edge(e[1],e[2]))){
            _edge[this->edge(e[1],e[2])] = mesh.v.size();
            mesh.v.push_back(VertItem((mesh.v[e[1]].mv+mesh.v[e[2]].mv)/2.0f));
        }
        if(_edge.end() == _edge.find(this->edge(e[0],e[2]))){
            _edge[this->edge(e[0],e[2])] = mesh.v.size();
            mesh.v.push_back(VertItem((mesh.v[e[0]].mv+mesh.v[e[2]].mv)/2.0f));
        }
    }

    decltype(mesh.f) _tf(mesh.f.size()*4,{0,0,0});
    for(int i=0;i<mesh.f.size(); i++){
        auto& e = mesh.f[i];
        _tf[4*i+0] = {e[0], _edge[this->edge(e[0],e[1])], _edge[this->edge(e[0],e[2])]};
        _tf[4*i+1] = {e[1], _edge[this->edge(e[1],e[2])], _edge[this->edge(e[1],e[0])]};
        _tf[4*i+2] = {e[2], _edge[this->edge(e[2],e[0])], _edge[this->edge(e[2],e[1])]};
        _tf[4*i+3] = {_edge[this->edge(mesh.f[i][0],mesh.f[i][1])],
                      _edge[this->edge(e[1],e[2])],
                      _edge[this->edge(e[2],e[0])]};
    }
    std::swap(_tf,mesh.f);
}
*/
