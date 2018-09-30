#include "ShaderController.h"
#include "ViewController.h"
#include "RenderController.h"

#include <glm/gtc/type_ptr.hpp>

template<>
LIBSHARED_EXPORT ShaderCtrl& global::con<ShaderCtrl>(){
    return ICtrl<ShaderCtrl>::getGrobalInstanceRef();
}

template<>
LIBSHARED_EXPORT ShaderCtrl& plugin::con<ShaderCtrl>(){
    return ICtrl<ShaderCtrl>::getPluginInstanceRef();
}

ShaderCtrl::ShaderCtrl()
{
}

ShaderCtrl::~ShaderCtrl()
{
    LOG(INFO) << "~ShaderCtrl";
    for(auto& e:program)
        delete e.second;
}

void ShaderCtrl::releaseShader(std::string name) {
//    delete program[name];
//    program.erase(program.find(name));
}


QOpenGLShaderProgram* ShaderCtrl::shader(std::string name) {
    auto result = program[name];
    if(result == nullptr)
        LOG(INFO) << "ShaderCtrl: invalid " << name;
    return result;
}

bool ShaderCtrl::addShaderProgram(std::initializer_list<ShaderProgramConfig> list) {
    for(auto& programConfig:list) {
        auto shader = new QOpenGLShaderProgram();
        for(auto e: programConfig.config)
            shader->addShaderFromSourceFile(e.first, e.second);
        if(programConfig.initial_f) programConfig.initial_f(shader->programId());
        shader->link();
        if(shader->isLinked() == false){
            qDebug() << shader->log();
            return false;
        }
        program[programConfig.name] = shader;
    }
    return true;
}

