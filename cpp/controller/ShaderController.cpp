#include "ShaderController.h"
#include "ViewController.h"
#include "RenderController.h"

#include <glm/gtc/type_ptr.hpp>

template<>
LIBSHARED_EXPORT ShaderCtrl& con<ShaderCtrl>(){
    return ICtrl<ShaderCtrl>::getInstanceRef();
}

ShaderCtrl::ShaderCtrl()
{
}

void ShaderCtrl::releaseShader(std::string name) {
    delete program[name];
    program.erase(program.find(name));
}

void ShaderCtrl::initialize(){
    QString prefix = PLUGINPATH+QString("GLViewer/glsl/");
//    addShaderProgram("indices", shaderConfig{
//                         V(prefix+"indices"),G(prefix+"indices"),F(prefix+"indices")
//                     });
    addShaderProgram("picker", shaderConfig{
                         V(prefix+"picker"),F(prefix+"picker")
                     });
    addShaderProgram("core", shaderConfig{
                         V(prefix+"default"),F(prefix+"default")
                     });
    addShaderProgram("selection", shaderConfig{
                         V(prefix+"default"),F(prefix+"selection")
                     });
    addShaderProgram("select", shaderConfig{
                         V(prefix+"select_mask"), G(prefix+"select_mask")
                     }, [](GLuint id){
        const char* select_varing[] = {"face"};
        gl.glTransformFeedbackVaryings(id, 1, select_varing ,GL_INTERLEAVED_ATTRIBS);
    });
}

QOpenGLShaderProgram* ShaderCtrl::shader(std::string name,bool use_core) {
    auto result = program[(use_core?"~":con<CentralCtrl>().module())+name];
    if(result == nullptr)
        LOG(INFO) << "ShaderCtrl: invalid " << (use_core?"~":con<CentralCtrl>().module())+name;
    return result;
}

bool ShaderCtrl::addShaderProgram(std::string name,std::map<QOpenGLShader::ShaderType,QString> shader_map,
                                     std::function<void(GLuint)> f){
    auto shader = new QOpenGLShaderProgram();
    for(auto e: shader_map)
        shader->addShaderFromSourceFile(e.first, e.second);
    if(f) f(shader->programId());
    shader->link();
    if(shader->isLinked() == false){
        qDebug() << shader->log();
        delete shader;
        return false;
    }
    this->program[con<CentralCtrl>().module()+name] = shader;
    con<CentralCtrl>().resourceHook(CentralCtrl::SHADER,name);
    return true;
}
