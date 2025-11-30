#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "glad/glad.h"
#include "VertexBuffer.h"

class VertexArray {
  private:
    GLuint id;

  public:
    VertexArray();

    void LinkAttrib(VertexBuffer vbo, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* offset);
    void Bind();
    void Unbind();
    void Delete();
};

#endif