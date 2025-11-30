#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "glad/glad.h"

class IndexBuffer {
  private:
    GLuint id;
  
  public:
    IndexBuffer(const GLuint* indicies, const GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif