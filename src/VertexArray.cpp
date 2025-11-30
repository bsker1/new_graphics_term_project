#include "VertexArray.h"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &id);
}

void VertexArray::LinkVertexBuffer(VertexBuffer vbo, const GLuint layout) {
  vbo.Bind();
  // Define layout of vbo data
  glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
