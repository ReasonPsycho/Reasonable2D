#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "spdlog/spdlog.h"
#include "glm/detail/type_vec3.hpp"
#include "glad/glad.h"


class Shader
{
public:
    // the program ID
    GLuint ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath)
            : vertexPath(vertexPath), fragmentPath(fragmentPath){}

    // use/activate the shader
    void use() const;
    void init();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setGLuint(const std::string &name, GLuint value) const;
    void setMatrix4(const std::string &name, bool transpose, const GLfloat *value) const;

    void setVec3(const std::string &name, float d, float d1, float d2);

private:
    const char* vertexPath{};
    const char* fragmentPath{};
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    static void checkCompileErrors(unsigned int shader, std::string type);

};

#endif
