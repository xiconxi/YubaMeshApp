#ifndef AUXILIARY_H
#define AUXILIARY_H
#include <string>
namespace YbCore {
namespace aux {

void addCoord3d(float h, float r, std::string name);
void addBox3d(float h, float r, std::string name);
void addInteractiveDemo(std::string filename, std::string name);
void addInteractiveFaceTexDemo(std::string filename, std::string name);

}
}


#endif // AUXILIARY_H
