#ifndef CAMERACtrl_H
#define CAMERACtrl_H

#include "../bases/ISingleton.inc"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QMatrix4x4>
#include "CentralController.h"

class LIBSHARED_EXPORT ViewerMatrix{
public:
    virtual glm::mat4 matrixV() {
        return  glm::ortho(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
//        return  glm::perspective(45.0f, ratio, 0.01f, 1000.0f);
    }

    virtual glm::mat4 matrixP() {
        return glm::lookAt(Pos, Pos + Front, Up);
    }

    virtual glm::mat4 matrixVP() {
        return matrixV()*matrixP();
    }

    QMatrix4x4 MatrixV() {
        return QMatrix4x4(glm::value_ptr(matrixV()), 4, 4);
    }

    QMatrix4x4 MatrixP() {
        return QMatrix4x4(glm::value_ptr(matrixP()), 4, 4);
    }

    QMatrix4x4 MatrixVP() {
        return QMatrix4x4(glm::value_ptr(matrixV()*matrixP()), 4, 4);
    }

    QMatrix4x4 Model() {
        return QMatrix4x4(glm::value_ptr(m_model),4,4);
    }

    glm::mat4 model() {
        return m_model;
    }

    void update_ratio(float _ratio) {
        ratio = _ratio;
    }

    void move(int x, int y);

    void rotate(int x,int y);

    void scaleBy(float s);

private:
    glm::mat4 m_model;
    glm::vec3 Pos   = glm::vec3(0.0f, 10.0f, 0.0f);
    glm::vec3 Front = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 Up    = glm::vec3(0.0f, 0.0f,  -1.0f);
    float ratio ;
};


class LIBSHARED_EXPORT ViewCtrl
{
public:
    void addView(std::string name, ViewerMatrix* matrix);

    void ActiveView(std::string name,bool use_core=false);

    void updateSize(int width, int height);

    void releaseView(std::string name);

    int width, height;

    ViewerMatrix* view();
private:
    friend class ICtrl<ViewCtrl>;
    std::map<std::string, ViewerMatrix*> viewMap;
    ViewerMatrix* activeView = nullptr;
    ViewCtrl(){}
    ~ViewCtrl(){}
};



#endif // CAMERACtrl_H
