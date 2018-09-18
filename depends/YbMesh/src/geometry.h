#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "types/type_mesh.h"
namespace YbMesh {
namespace geometry {
indicesTriMesh<glm::vec3> make_spherical(int scale);
indicesTriMesh<glm::vec3> LIBSHARED_EXPORT make_axes(float h,float r);
indicesTriMesh<glm::vec3> LIBSHARED_EXPORT make_box(float h,float r);
}
}
#endif // GEOMETRY_H
