#ifndef TYPE_MESH_H
#define TYPE_MESH_H

#include <vector>
#include <ostream>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <algorithm>

namespace YbMesh {
using std::vector;
    template <class T>
    struct indicesTriMesh
    {
        indicesTriMesh(std::shared_ptr<vector<T>> _vp, std::shared_ptr<vector<glm::ivec3>> _fp)
            :vp(_vp),fp(_fp){}

        indicesTriMesh(vector<T>& _v, vector<glm::ivec3>& _f)
            :vp(std::shared_ptr<vector<T> >(&_v)),fp(std::shared_ptr<vector<glm::ivec3> >(&_f)){}

        vector<T>& v(){
            return *vp;
        }
        vector<glm::ivec3>& f(){
            return *fp;
        }
    private:
        std::shared_ptr<vector<T>> vp;
        std::shared_ptr<vector<glm::ivec3>> fp;
    };
}


// old version
namespace glm {
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
}

#endif // TYPE_MESH_H
