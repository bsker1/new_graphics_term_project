#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>

std::string readFile(const std::string &filePath);

class Shader {
  private:
    GLuint id;

  public:
    Shader(const std::string vertexFile, const std::string fragmentFile);

    void Activate();
    void Delete();
};

#endif