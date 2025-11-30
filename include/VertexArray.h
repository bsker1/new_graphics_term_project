#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "glad/glad.h"
#include "VertexBuffer.h"

class VertexArray {
  private:
    GLuint id;

  public:
    VertexArray();

    void LinkVertexBuffer(VertexBuffer vbo, const GLuint layout);
    void Bind();
    void Unbind();
    void Delete();
};

#endif