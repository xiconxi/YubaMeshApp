#ifndef SHADERCtrl_H
#define SHADERCtrl_H

#include "../bases/ISingleton.inc"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "CentralController.h"

class LIBSHARED_EXPORT ShaderCtrl
{
public:
    void initialize();
    QOpenGLShaderProgram* shader(std::string name,bool use_core=false);
    bool addShaderProgram(std::string name,std::map<QOpenGLShader::ShaderType,QString> shader_map,
                          std::function<void(GLuint)> f=nullptr);

    void releaseShader(std::string name);

private:
    std::map<std::string,QOpenGLShaderProgram*> program;
    friend class ICtrl<ShaderCtrl>;
    ShaderCtrl();
};
typedef std::map<QOpenGLShader::ShaderType,QString> shaderConfig;

#define V(x) {QOpenGLShader::Vertex,   x+".v.glsl"}
#define G(x) {QOpenGLShader::Geometry,   x+".g.glsl"}
#define F(x) {QOpenGLShader::Fragment,   x+".f.glsl"}

#endif // SHADERCtrl_H
