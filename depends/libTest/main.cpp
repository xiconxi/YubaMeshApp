#include <iostream>
#include <meshcodecs/format_obj.h>
#include <easylogging++.h>

using std::shared_ptr;
using std::cout;

int main() {
    auto mesh = YbMesh::IO::importOBJ_V1("/Users/hotpot/data/mesh_data/body2.obj").second;
    LOG(INFO) << mesh.v().size() << ' ' << mesh.f().size() ;
}
