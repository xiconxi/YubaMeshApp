#include "geometry.h"
#include <cmath>
#include <iostream>
#include <array>
#include <easylogging++.h>
#define PI 3.1415926f
YbMesh::indicesTriMesh<glm::vec3> YbMesh::geometry::make_spherical(int rows) {
    const float pi = 3.1415926f;
    using namespace std;
    auto vs = make_shared<vector<glm::vec3>>();
    auto fs = make_shared<vector<glm::ivec3>>();
    vs->emplace_back(glm::vec3(.0f,.0f, 1.0f));
    for(float j = pi; j >= 0.0f; j -= pi/rows){
        for(float i = 2*pi; i >= 0.0f; i -= 2*pi/rows) {
            vs->emplace_back(glm::vec3(sinf(j)*cosf(i), sinf(j)*sinf(i),cosf(j)));
        }
    }
    return YbMesh::indicesTriMesh<glm::vec3>(vs,fs);
}

YbMesh::indicesTriMesh<glm::vec3> YbMesh::geometry::make_cylinders(float radius,int scale) {

}

YbMesh::indicesTriMesh<glm::vec3> YbMesh::geometry::make_axes(float height,float r) {
    auto v = std::make_shared<std::vector<glm::vec3>>();
    auto f = std::make_shared<std::vector<glm::ivec3>>();

    // z
    for(float delta = 2*PI; delta >= 0; delta -= PI/10)
        v->emplace_back(glm::vec3(2*r*std::sinf(delta), 2*r*std::cosf(delta), height+r*2));

    for(float h = 1.0f; h >= 0; h -= 1.0f/4) {
        for(float delta = 2*PI; delta >= 0; delta -= PI/10)
            v->emplace_back(glm::vec3(r*std::sinf(delta), r*std::cosf(delta), height*h+r*2));
    }
    v->emplace_back(glm::vec3(.0f, .0f, height+r*5));
    v->emplace_back(glm::vec3(.0f, .0f, 0.0f));

    // y
    for(float delta = 2*PI; delta >= 0; delta -= PI/10)
        v->emplace_back(glm::vec3(2*r*std::cosf(delta), height+r*2, 2*r*std::sinf(delta)));

    for(float h = 1.0f; h >= 0; h -= 1.0f/4) {
        for(float delta = 2*PI; delta >= 0; delta -= PI/10)
            v->emplace_back(glm::vec3(r*std::cosf(delta), height*h+r*2, r*std::sinf(delta)));
    }
    v->emplace_back(glm::vec3(.0f, height+r*5, .0f));
    v->emplace_back(glm::vec3(.0f, 0.0f, .0f));

    // x
    for(float delta = 2*PI; delta >= 0; delta -= PI/10)
        v->emplace_back(glm::vec3(height+r*2, 2*r*std::sinf(delta),  2*r*std::cosf(delta)));

    for(float h = 1.0f; h >= 0; h -= 1.0f/4) {
        for(float delta = 2*PI; delta >= 0; delta -= PI/10)
            v->emplace_back(glm::vec3(height*h+r*2, r*std::sinf(delta), r*std::cosf(delta)));
    }
    v->emplace_back(glm::vec3(height+r*5, .0f, .0f));
    v->emplace_back(glm::vec3(.0f, 0.0f, .0f));

    for(int offset:{0,122,244}){
        for(int j = 0; j < 20; j++) {
            // top-down faces
            f->emplace_back(glm::ivec3(j,(j+1)%20,120)+offset);
            f->emplace_back(glm::ivec3(100+(j+1)%20,100+j,121)+offset);
            for(int i = 0; i < 5; i++) {
                f->emplace_back(glm::ivec3(i*20+(j+1)%20,i*20+j,(1+i)*20+j)+offset);
                f->emplace_back(glm::ivec3((i+1)*20+j,(i+1)*20+(j+1)%20,i*20+(j+1)%20)+offset);
            }
        }
    }

    return YbMesh::indicesTriMesh<glm::vec3>(v,f);
}

YbMesh::indicesTriMesh<glm::vec3> YbMesh::geometry::make_box(float height,float r) {
    auto v = std::make_shared<std::vector<glm::vec3>>();
    auto f = std::make_shared<std::vector<glm::ivec3>>();

    // z directions
    for(int e:{0,1,2,3}) {
        glm::vec3 offset = 0.5f*height*glm::vec3(e&2?-1.0f:1.0f,e&1?-1.0f:1.0f,-1.0f);
        for(float h = 1.0f; h >= 0; h -= 1.0f/4) {
            for(float delta = 2*PI; delta >= 0; delta -= PI/10)
                v->emplace_back(offset+glm::vec3(r*std::sinf(delta), r*std::cosf(delta), r+(height-2*r)*h));
        }
    }

    // y
    for(int e:{0,1,2,3}) {
        glm::vec3 offset = 0.5f*height*glm::vec3(e&2?-1.0f:1.0f,-1.0f,e&1?-1.0f:1.0f);
        for(float h = 1.0f; h >= 0; h -= 1.0f/4) {
            for(float delta = 2*PI; delta >= 0; delta -= PI/10)
                v->emplace_back(offset+glm::vec3(r*std::cosf(delta), r+(height-2*r)*h, r*std::sinf(delta)));
        }
    }

    // x
    for(int e:{0,1,2,3}) {
        glm::vec3 offset = 0.5f*height*glm::vec3(-1.0f,e&2?-1.0f:1.0f,e&1?-1.0f:1.0f);
        for(float h = 1.0f; h >= 0; h -= 1.0f/4) {
            for(float delta = 2*PI; delta >= 0; delta -= PI/10)
                v->emplace_back(offset+glm::vec3(r+(height-2*r)*h, r*std::sinf(delta), r*std::cosf(delta)));
        }
    }

    for(int offset = 0; offset < 1200; offset += 100){
        for(int j = 0; j < 20; j++) {
            for(int i = 0; i < 4; i++) {
                f->emplace_back(offset+glm::ivec3(i*20+(j+1)%20,i*20+j,(1+i)*20+j));
                f->emplace_back(offset+glm::ivec3((i+1)*20+j,(i+1)*20+(j+1)%20,i*20+(j+1)%20));
            }
        }
    }

    for(int e = 0; e< 8; e++) {
        v->emplace_back(0.5f*height*glm::vec3(e&4?-1.0f:1.0f,e&2?-1.0f:1.0f,e&1?-1.0f:1.0f));
        for(int j = 0; j < 20; j++) {
            f->emplace_back(glm::ivec3(j+800+80*bool(e&4)+100*(e&3),(j+1)%20+800+80*bool(e&4)+100*(e&3),1200+e));
            f->emplace_back(glm::ivec3(j+400+80*bool(e&2)+100*(((e&4)>>1)+(e&1)),(j+1)%20+400+80*bool(e&2)+100*(((e&4)>>1)+(e&1)),1200+e));
            f->emplace_back(glm::ivec3(j+ 00+80*bool(e&1)+100*((e&6)>>1),(j+1)%20+ 00+80*bool(e&1)+100*((e&6)>>1),1200+e));
        }
    }

    return YbMesh::indicesTriMesh<glm::vec3>(v,f);
}
