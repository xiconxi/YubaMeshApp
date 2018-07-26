#ifndef DEV_MESHPROCESSING_H
#define DEV_MESHPROCESSING_H

#include "../glviewer/mesh/meshCodes.h"
#include <vector>
#include <array>

bool slice_in_triangel(glFormatMesh* mesh,glm::ivec3 f, glm::vec3 n, float d);

std::array<glm::vec3,2> slice_triangel_line(glFormatMesh* mesh,glm::ivec3 f, glm::vec3 n, float d);

void sortByVector(glFormatMesh* mesh, glm::vec3 x);


#endif // DEV_MESHPROCESSING_H
