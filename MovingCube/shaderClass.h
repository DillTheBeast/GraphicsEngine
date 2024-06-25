#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID;
    Shader(const char* vertexPath, const char* fragmentPath);

    void Activate();
    void Delete();

    void SetMat4(const std::string &name, const glm::mat4 &mat) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetInt(const std::string &name, int value) const;
};

#endif
