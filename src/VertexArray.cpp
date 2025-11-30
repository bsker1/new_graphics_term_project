#include "VertexArray.h"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &id);
}

void VertexArray::LinkAttrib(VertexBuffer vbo, const GLuint layout, const GLuint numComponents, const GLenum type, const GLsizeiptr stride, const void* offset) {
  vbo.Bind();
  // Define layout of vbo data
  glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  vbo.Unbind();
}

void VertexArray::Bind() {
  glBindVertexArray(id);
}

void VertexArray::Unbind() {
  glBindVertexArray(0);
}

void VertexArray::Delete() {
  glDeleteVertexArrays(1, &id);
}
