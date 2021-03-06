#ifndef TYPE_MESH_H
#define TYPE_MESH_H

#include <vector>
#include <ostream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <memory>
#include <iostream>
#include <QtCore/QtGlobal>

#ifndef LIBSHARED_EXPORT
#if defined(YBMESH_LIBRARY)
#  define LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSHARED_EXPORT Q_DECL_IMPORT
#endif
#endif

namespace YbMesh {
    using std::vector;
    template <class T>
    using share_vec =  std::shared_ptr<std::vector<T>>;
    template <class T>
    struct indicesTriMesh
    {
        indicesTriMesh(const indicesTriMesh<T>& m):vp(m.vp),fp(m.fp){}
        indicesTriMesh(const indicesTriMesh<T>&& m):vp(m.vp),fp(m.fp){}
        indicesTriMesh(std::shared_ptr<vector<T>> _vp, std::shared_ptr<vector<glm::ivec3>> _fp)
            :vp(_vp),fp(_fp){}

        ~indicesTriMesh() {
            std::cout << "~indicesTriMesh" << std::endl;
        }

        vector<T>& v(){
            return *vp;
        }
        vector<glm::ivec3>& f(){
            return *fp;
        }
        std::shared_ptr<vector<T>> shared_v() const{
            return vp;
        }
        std::shared_ptr<vector<glm::ivec3>> shared_f() const {
            return fp;
        }
    private:
        std::shared_ptr<vector<T>> vp;
        std::shared_ptr<vector<glm::ivec3>> fp;
    };

}


// old version
namespace glm {

    template<typename T, precision P = defaultp>
    inline std::ostream &operator<<(std::ostream &os, const tvec4<T, P> &e) {
        return os << e.r << ' ' << e.g << ' ' << e.b << ' ' << e.a;
    }

    template<typename T, precision P = defaultp>
    inline std::ostream &operator<<(std::ostream &os, const tvec3<T, P> &e) {
        return os << e.r << ' ' << e.g << ' ' << e.b;
    }
    template<typename T, precision P = defaultp>
    inline std::ostream &operator<<(std::ostream &os, const tvec2<T, P> &e) {
#ifdef MESH_EXPORT_CPP
        return os << e.r << ' ' << e.g << " 0";
#else
        return os << e.r << ' ' << e.g;
#endif
    }
    template<typename T, precision P = defaultp>
    inline std::ostream &operator<<(std::ostream &os, const tmat4x4<T, P> &s) {
        auto e = glm::transpose(s);
        return os <<'\n' <<  e[0] << '\n' << e[1] << '\n' << e[2] << '\n' << e[3];
    }
    template<typename T, precision P = defaultp>
    inline std::ostream &operator<<(std::ostream &os, const tmat3x3<T, P> &s) {
        auto e = glm::transpose(s);
        return os <<'\n' <<  e[0] << '\n' << e[1] << '\n' << e[2];
    }
}

#endif // TYPE_MESH_H
