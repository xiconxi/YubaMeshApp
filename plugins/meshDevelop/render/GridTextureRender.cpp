#include "GridTextureRender.h"
#include <QImage>
#include <Eigen/Sparse>
#include <YbCore/scripts>
#define PI 3.1415926f

namespace paramarization {
    std::vector<int> borderExtraction(YbMesh::SharedHalfEdge& heMesh){
         std::ofstream file("/Users/hotpot/data/mesh_data/body2_head_border.obj",std::ios_base::out);
         for(int i = 0; i< heMesh.verts.size(); i++)
             file << "v " << heMesh.verts[i].v  << "\n";
         std::vector<int> m;
         // Half Edge Implement
         auto null_it = heMesh.edges.end();
         auto s_edge = std::find_if(heMesh.edges.begin(), heMesh.edges.end(),
                                    [=](YbMesh::IHalfEdge& it){ return it._pair == -1; });
         auto curr = s_edge;
         do{
             m.emplace_back(curr->_to);
             file << "l " << curr->_from+1 << ' ' << curr->_to+1 << "\n";
             while(curr->next()->pair() != null_it)
                 curr = curr->next()->pair();
             curr = curr->next();
         }while(s_edge != curr);
         return m;
    }

    std::pair<std::vector<float>,std::vector<float>> make_circle(int verts){
        std::vector<float> u,v;
        u.reserve(verts); v.reserve(verts);
        for(float alpha = 0; alpha < 2*PI; alpha += 2*PI/verts ){
            u.emplace_back(std::sinf(alpha));
            v.emplace_back(std::cosf(alpha));
        }

        return std::make_pair(u,v);
    }

    YbMesh::share_vec<glm::vec2> test(YbMesh::indicesTriMesh<glm::vec3>& mesh) {
        auto heMesh = YbMesh::SharedHalfEdge(mesh);
        auto n = mesh.v().size();
        auto m = heMesh.rerange(paramarization::borderExtraction(heMesh));
        m = heMesh.rerange(paramarization::borderExtraction(heMesh));
        auto circle = paramarization::make_circle(m);

        // Laplace Matrix
        std::vector<float> w_ii(n,0.0f);
        std::vector<Eigen::Triplet<float>> border_triplet;
        std::vector<Eigen::Triplet<float>> inner_triplet;
        for(auto& edge:heMesh.edges) {
            if(edge._from < m) continue;
            if(edge._to < m){ // L(border,inner)
                border_triplet.emplace_back(Eigen::Triplet<float>{ edge._from-m, edge._to, 1.0f});
            }else if(edge._to >= m) { // L(inner,inner)
                inner_triplet.emplace_back(Eigen::Triplet<float>{ edge._from-m, edge._to-m, 1.0f});
            }
            w_ii[edge._from] -= 1.0f;
        }
        for(int i = m; i < n; i++)
            inner_triplet.emplace_back(Eigen::Triplet<float>{ i-m,i-m,w_ii[i]});

        Eigen::SparseMatrix<float> inner(n-m, n-m);
        Eigen::SparseMatrix<float> border(n-m, m);
        inner.setFromTriplets(inner_triplet.begin(), inner_triplet.end());
        border.setFromTriplets(border_triplet.begin(), border_triplet.end());

        // linear system
        Eigen::VectorXf c_u(m), p_u(n-m), c_v(m), p_v(n-m);
        for(int i = 0; i < m; i++) {
            c_u[i] = circle.first[i];
            c_v[i] = circle.second[i];
        }
        Eigen::SparseLU<Eigen::SparseMatrix<float>> solver;
        solver.analyzePattern(inner);
        solver.factorize(inner);
        p_u = solver.solve(border*c_u);
        p_v = solver.solve(border*c_v);

        auto tex = std::make_shared<std::vector<glm::vec2>>(n);
        for(int i = 0; i < m; i++) (*tex)[i] = glm::vec2(c_u[i]  , c_v[i]  );
        for(int i = m; i < n; i++) (*tex)[i] = glm::vec2(p_u[i-m], p_v[i-m]);

        return tex;
    }

    YbMesh::share_vec<glm::vec2> fixBoundParamarization(YbMesh::indicesTriMesh<glm::vec3>& mesh) {
        auto heMesh = YbMesh::SharedHalfEdge(mesh);
        auto borderi = paramarization::borderExtraction(heMesh);
        auto n = mesh.v().size();
        auto m = borderi.size(); // heMesh.rerange(std::move(borderi));
        auto circle = paramarization::make_circle(m);
        auto tex = std::make_shared<std::vector<glm::vec2>>(n);

        // 划分边界点与内点[0,m) [m,n-1)
        std::vector<glm::ivec2> dual_indices(n,glm::ivec2(-1));

        for(int i = 0; i < m; i++) {
            dual_indices[i][0] = borderi[i];
            dual_indices[borderi[i]][1] = i;
        }
        for(int i = 0, j = m; j < n; i++, j++) {
            while(dual_indices[i][1] != -1) i++;
            dual_indices[j][0] = i;
            dual_indices[i][1] = j;
        }

        // Laplace Matrix
        std::vector<float> w_ii(n,0.0f);
        std::vector<Eigen::Triplet<float>> border_triplet;
        std::vector<Eigen::Triplet<float>> inner_triplet;
        for(auto& edge:heMesh.edges) {
            int from = dual_indices[edge._from][1];
            int to = dual_indices[edge._to][1];
            if(from < m) continue;
            if(to  < m){ // L(border,inner)
                border_triplet.emplace_back(Eigen::Triplet<float>{ from-m, to, 1.0f});
            }else if(to >= m) { // L(inner,inner)
                inner_triplet.emplace_back(Eigen::Triplet<float>{ from-m, to-m, 1.0f});
            }
            w_ii[from] -= 1.0f;

//            w_ii[to] -= 1.0f;
        }
        for(int i = m; i < n; i++)
            inner_triplet.emplace_back(Eigen::Triplet<float>{ i-m,i-m,w_ii[i]});

        Eigen::SparseMatrix<float> inner(n-m, n-m);
        Eigen::SparseMatrix<float> border(n-m, m);

        inner.setFromTriplets(inner_triplet.begin(), inner_triplet.end());
        border.setFromTriplets(border_triplet.begin(), border_triplet.end());

        // linear system
        Eigen::VectorXf c_u(m), p_u(n-m), c_v(m), p_v(n-m);
        for(int i = 0; i < m; i++) {
            c_u[i] = circle.first[i];
            c_v[i] = circle.second[i];
        }
        Eigen::SparseLU<Eigen::SparseMatrix<float>> solver;
        solver.analyzePattern(inner);
        solver.factorize(inner);
        p_u = solver.solve(border*c_u);
        p_v = solver.solve(border*c_v);

        for(int i = 0; i < m; i++) (*tex)[dual_indices[i][0]] = -glm::vec2(c_u[i]  , c_v[i]  );
        for(int i = m; i < n; i++) (*tex)[dual_indices[i][0]] = glm::vec2(p_u[i-m], p_v[i-m]);

        auto new_f = std::make_shared<std::vector<glm::ivec3>>();
        for(auto& f:mesh.f())
            new_f->emplace_back(f);

        YbMesh::IO::exportObj("/Users/hotpot/data/mesh_data/paramarization_tex1.obj", new_f, tex);

        return tex;

    }


}

GridTextureRender::GridTextureRender(TriMesh& mesh):IGLMeshObject(std::move(mesh)),
    vt_p(paramarization::fixBoundParamarization(mesh)),
    RenderScript(YbCore::defaultScript::nullRender)
{
    this->normalize();
    this->calculateNorm();
    RenderScript([this](QTime&) {
        this->texture = new  QOpenGLTexture(QImage("/Users/hotpot/Downloads/wechat_head.jpg"));
        this->texture->bind();
        this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
        this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
        this->texture->setWrapMode(QOpenGLTexture::Repeat);
        this->texture->release();
        this->createBufferScript();
        this->syncVertexBuffersDataScript();
        this->syncFacesBuffersDataScript();
    });
    setRender(std::bind(&GridTextureRender::draw, this, std::placeholders::_1));
}

void GridTextureRender::draw(QTime &t) {
    auto shader = plugin::con<ShaderCtrl>().shader("grid_tex");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*this->Model());
    this->drawElementScript();
    texture->release();
    shader->release();
//    global::con<RenderCtrl>().update(true);
}

void GridTextureRender::syncVertexBuffersDataScript() {
    auto& f = m_v.f();
    auto& v = m_v.v();
    auto& vn = m_n.v();
    auto& vt = *vt_p;
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    gl.glBufferData(GL_ARRAY_BUFFER, v.size()*(sizeof(v[0])+sizeof(vn[0])+sizeof(vt[0])), nullptr, GL_STATIC_DRAW);

    gl.glBufferSubData(GL_ARRAY_BUFFER, 0, v.size()*sizeof(v[0]), v.data());
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v[0]), (GLvoid*)0);
    gl.glEnableVertexAttribArray(0);

    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), vn.size()*sizeof(vn[0]), vn.data());
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vn[0]), (GLvoid*)(v.size()*sizeof(v[0])));
    gl.glEnableVertexAttribArray(1);

    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0])+vn.size()*sizeof(vn[0]), vt.size()*sizeof(vt[0]), vt.data());
    gl.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vt[0]), (GLvoid*)(v.size()*sizeof(v[0]) + vn.size()*(sizeof(vn[0]))));
    gl.glEnableVertexAttribArray(2);

    gl.glBindVertexArray(0);
}
