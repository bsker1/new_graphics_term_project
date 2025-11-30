#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <string>

std::string readFile(const std::string &filePath);

class Shader {
  private:
    GLuint id;

  public:
    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();
};

#endif