#include "MeshProcessing.h"
#include <fstream>
#include <easylogging++.h>

std::vector<std::array< std::vector<std::array<glm::vec3,2>>::iterator,2>> plugin::_RFF::sortContours(std::vector<std::array<glm::vec3,2>>&& intersections) {
    std::vector<std::array< std::vector<std::array<glm::vec3,2>>::iterator,2>> intervals = {{intersections.begin(),intersections.begin()}};
    for(auto it = intersections.begin(); it != intersections.end(); it++) {
        auto find_it = std::find_if(std::next(it), intersections.end(), [=](std::array<glm::vec3,2>& e){
            return  (e[0][0] == (*it)[1][1] && e[0][1] == (*it)[1][0]) || (e[0][0] == (*it)[0][1] && e[0][1] == (*it)[0][0])
                    ||(e[1][0] == (*it)[0][1] && e[1][1] == (*it)[0][0]) || (e[1][0] == (*it)[1][1] && e[1][1] == (*it)[1][0]);
        });
        if(find_it != intersections.end()) {
            std::swap(*std::next(it),*find_it);
        }else{
            (*std::prev(intervals.end()))[1] = std::next(it);
            if(it != std::prev(intersections.end()))
                intervals.push_back({std::next(it),std::next(it)});
        }
    }
    return intervals;
}

void plugin::_RFF::writeSlices(PickableMesh* mesh, std::vector<std::array<glm::vec3,2>>&& slices, std::string file_name){
    std::fstream fileHandle;
    fileHandle.open(file_name,std::fstream::out|| std::ofstream::app);
    for(auto it = slices.begin(); it != slices.end(); it++) {
        std::array<glm::vec3,2> line {
            mesh->v[(*it)[0][0]]+(*it)[0][2]*(mesh->v[(*it)[0][1]] - mesh->v[(*it)[0][0]]),
            mesh->v[(*it)[1][0]]+(*it)[1][2]*(mesh->v[(*it)[1][1]] - mesh->v[(*it)[1][0]])
        };
        fileHandle << "v " << line[0][0] << ' ' << line[0][1] << ' ' << line[0][2] << std::endl;
        fileHandle << "v " << line[1][0] << ' ' << line[1][1] << ' ' << line[1][2] << std::endl;
        fileHandle << "l " << 2*std::distance(it,slices.begin())+1 << ' ' << 2*std::distance(it,slices.begin())+2 << std::endl;
    }
    fileHandle.close();
}

void plugin::_RFF::writePartialMesh(PickableMesh *mesh, std::vector<glm::ivec3> &&faces, std::string file_name) {
    std::map<int,int> v_rank;
    std::fstream fileHandle;
    fileHandle.open(file_name,std::ofstream::out || std::ofstream::app);
    for(auto it = faces.begin(); it != faces.end(); it++) {
        for(auto& i:{0,1,2}){
            if(v_rank.count((*it)[i])) continue;
            auto& v = mesh->v[(*it)[i]];
            v_rank[(*it)[i]] = v_rank.size()+1;
            fileHandle << "v " << v[0] << ' ' << v[1] << ' ' << v[2] << std::endl;
        }
        fileHandle << "f " << v_rank[(*it)[0]] << ' ' << v_rank[(*it)[1]] << ' ' << v_rank[(*it)[2]] << std::endl;
    }
    fileHandle.close();
}

///
/// \brief plugin::extractMeshBorder
/// \param mesh
/// \param faces
/// 网格边界提取
std::vector<glm::ivec2> plugin::_RFF::extractMeshBorder(PickableMesh *mesh, std::vector<glm::ivec3> &&faces) {
    auto half_edges = YbCore::PairKeyMap<uint,uint>([](const YbCore::PairKey<int>& e1, const YbCore::PairKey<int>& e2){
        return (e1.first == e2.first)?(e1.second > e2.second):(e1.first > e2.first);
    });
    std::vector<int> faceRefCnt(faces.size(), 0);
    // 提取所有独享边
    for(auto it = faces.begin(); it != faces.end(); it++) {
        for(auto i:{0,1,2}) {
            auto pair = std::make_pair((*it)[i==2?0:i+1], (*it)[i]);
            auto self = std::make_pair((*it)[i], (*it)[i==2?0:i+1]);
            if( half_edges.count(pair)){
                auto Kpair = half_edges.find(pair);
                faceRefCnt[(*Kpair).second]++;
                faceRefCnt[it - faces.begin()]++;
                half_edges.erase(Kpair);
            }else{
                half_edges[self] = it - faces.begin();
            }
        }
    }
    std::vector<glm::ivec2> borders;
    for(auto it = half_edges.begin(); it != half_edges.end(); it++) {
        borders.emplace_back(glm::ivec2(it->first.first,it->first.second));
    }
    return borders;
}
