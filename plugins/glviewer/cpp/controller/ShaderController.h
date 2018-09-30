#ifndef SHADERCtrl_H
#define SHADERCtrl_H

#include "../bases/ISingleton.inc"
#include <QOpenGLShader>
#include <array>
#include <QOpenGLShaderProgram>
#include "CentralController.h"

typedef std::map<QOpenGLShader::ShaderType,QString> GLSLFileConfig;
struct ShaderProgramConfig
{
    std::string name;
    GLSLFileConfig config;
    std::function<void(GLuint)> initial_f;
};

class LIBSHARED_EXPORT ShaderCtrl
{
public:
    ~ShaderCtrl();
    QOpenGLShaderProgram* shader(std::string name);
    bool addShaderProgram(std::initializer_list<ShaderProgramConfig> list);

    void releaseShader(std::string name);

private:
    std::map<std::string, QOpenGLShaderProgram* > program;
    friend class ICtrl<ShaderCtrl>;
    ShaderCtrl();
};

#define V(x) {QOpenGLShader::Vertex,   x+".v.glsl"}
#define G(x) {QOpenGLShader::Geometry,   x+".g.glsl"}
#define F(x) {QOpenGLShader::Fragment,   x+".f.glsl"}

#endif // SHADERCtrl_H
