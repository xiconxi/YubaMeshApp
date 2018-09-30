#include "ViewController.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void ViewerMatrix::move(int x, int y){
    if (abs(x) < abs(y)) {
        Pos -= Up * (0.002f * y);
    } else {
        Pos += glm::cross(Up, Front) * (0.002f * x);
    }
}
/*
 *   z
 *   |__ y
 * x/
 *
 */
void ViewerMatrix::rotate(int dx, int dy) {
    if (abs(dx) > abs(dy)) {
        m_model = glm::rotate(glm::mat4(), dx * 0.003f, Up) * m_model;
    } else if (abs(dy) > abs(dx) ) {
        m_model = glm::rotate(glm::mat4(), dy * 0.003f, glm::cross(Up, Front)) * m_model;
    }
}

void ViewerMatrix::scaleBy(float s) {
    m_model = glm::scale(glm::mat4(),(1+s)*glm::vec3(1.0f)) * m_model;
}

template<>
LIBSHARED_EXPORT ViewCtrl& global::con<ViewCtrl>(){
    return ICtrl<ViewCtrl>::getGrobalInstanceRef();
}

void ViewCtrl::updateSize(int _width, int _height){
    this->width = _width;
    this->height = _height;
    for(auto& e:viewMap)
        e.second->update_ratio(width/(float)height);
}


void ViewCtrl::addView(std::string name, ViewerMatrix *matrix) {
    if(viewMap.find(name) != viewMap.end())
        delete viewMap[name];
    viewMap[name] = matrix;
    if(activeView == nullptr) activeView = matrix;
}

void ViewCtrl::releaseView(std::string name) {
    delete viewMap[name];
    viewMap.erase(viewMap.find(name));
}

void ViewCtrl::ActiveView(std::string name,bool use_core) {
    activeView =viewMap[name];
}

ViewerMatrix* ViewCtrl::view(){
    return activeView;
}
