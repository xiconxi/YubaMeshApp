#include "WhirlwindRender.h"
#include <QTime>
#include <stack>
#include <YbCore/scripts>

WhirlwindRender::WhirlwindRender(TriMesh&& vmesh): IDrawObject(std::move(vmesh)),
    RenderScript(std::bind(&WhirlwindRender::draw, this, std::placeholders::_1))
{
    this->normalize();
    this->calculateNorm();

    RenderScript([this](QTime&) {
        this->createBufferScript();
        this->syncVertexBuffersDataScript();
        this->syncFacesBuffersDataScript();
    });
}

void WhirlwindRender::draw(QTime &t) {
    static int s = 0;
    if(this->visible == false) return;
    auto shader = global::con<ShaderCtrl>().shader("core");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("base_color", 0.2f,0.2f,0.2f);
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*this->Model());
    this->drawElementScript(0, s %m_v.f().size() + 1);
    shader->release();
    global::con<RenderCtrl>().update(true);
    s += m_v.f().size()/1000;
}

///
/// \brief invincibleWhirlwindTriangle
/// \param mesh
/// 三角网格面元螺旋展开(面元的全排序)
void invincibleWhirlwindTriangle(YbMesh::indicesTriMesh<glm::vec3>& mesh) {
    auto half = YbMesh::SharedHalfEdge(mesh);
    std::vector<bool> face_visited_flag(mesh.f().size(),false);
    std::vector<glm::ivec3> new_f;
    new_f.reserve(mesh.f().size());

    for(int last_face = 0; last_face < mesh.f().size(); last_face++) {
        if(face_visited_flag[last_face]) continue;
        new_f.emplace_back(mesh.f()[last_face]);
        face_visited_flag[last_face] = true;
        YbMesh::IHalfEdgeIt last_edge = half.edge(last_face*3);
        std::stack<int> border_edges;
        while(1) {
            if(face_visited_flag[last_edge->pair()->face()]) {
                if(face_visited_flag[last_edge->next()->pair()->face()]){
                    while(border_edges.empty() == false){
                        auto e = half.edge(border_edges.top());
                        border_edges.pop();
                        if(face_visited_flag[e->pair()->face()] == false){
                            last_edge = e;
                            LOG(INFO) << "fake";
                            break;
                        }
                    }
                    if(border_edges.empty())
                    break;
                }else
                    last_edge = last_edge->next();
            }
            last_edge = last_edge->pair();
            new_f.emplace_back(mesh.f()[last_edge->face()]);
            face_visited_flag[last_edge->face()] = true;
            last_edge = last_edge->next();

            border_edges.push(last_edge->_next);
        }
    }
    std::swap(mesh.f(),new_f);
}

