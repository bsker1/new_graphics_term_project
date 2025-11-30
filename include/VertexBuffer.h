#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "glad/glad.h"

class VertexBuffer {
  private:
    GLuint id;
  
  public:
    VertexBuffer(GLfloat* verticies, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif