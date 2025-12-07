#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include "Shader.h"

class Texture {
  private:
    GLuint id;
    GLenum type;
    GLuint unit;

  public:
    Texture(const std::string imgFilePath, const GLenum texType, const GLuint slot, const GLenum format);

    void TexUnit(Shader& shaderProgram, const char* uniform, const GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
};

#endif