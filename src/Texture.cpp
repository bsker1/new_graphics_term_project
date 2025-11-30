#include "Texture.h"

#include "stb/stb_image.h"

Texture::Texture(const std::string imgFilePath, const GLenum texType, const GLenum slot, const GLenum format) {
  // Assign texture type
  type = texType;

  // Load in provided texture with stb_image
  int imgWidth, imgHeight, imgNumColorChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* imgBytes = stbi_load(imgFilePath.c_str(), &imgWidth, &imgHeight, &imgNumColorChannels, 0);

  // Assign provided texture to OpenGl texture
  glGenTextures(1, &id);
  glActiveTexture(slot);
  glBindTexture(texType, id);

  // Configure algorithm for resizing image
  glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Configure texture repeat
  glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Configuration for GL_CLAMP_TO_BORDER
  //float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

  // Applies image to OpenGL texture
  glTexImage2D(texType, 0, GL_RGBA, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, imgBytes);
  // Generate mipmaps for image downscaling
  glGenerateMipmap(texType);

  // Delete image data after loaded in OpenGL
  stbi_image_free(imgBytes);
  // Unbind texture
  glBindTexture(texType, 0);
}

void Texture::TexUnit(Shader& shaderProgram, const char* uniform, const GLuint unit) {
  // Locate provided uniform in shader program
  GLuint uniTex = glGetUniformLocation(shaderProgram.GetID(), uniform);
  // Activate shader to change uniform value
  shaderProgram.Activate();
  // Change uniform value
  glUniform1i(uniTex, unit);
}

void Texture::Bind() {
  glBindTexture(type, id);
}

void Texture::Unbind() {
  glBindTexture(type, 0);
}

void Texture::Delete() {
  glDeleteTextures(1, &id);
}
