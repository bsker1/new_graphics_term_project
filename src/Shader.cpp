#include "Shader.h"

#include <fstream>

// Output file contents given a file path
static std::string readFile(const std::string &filePath) {
  std::ifstream stream(filePath);
  std::string currentLine;
  std::string fileContents;
  while (getline(stream, currentLine)) {
    fileContents += currentLine + '\n';
  }

  return fileContents;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
  // get vertex shader source code into char array
  std::string vertexContents = readFile(vertexFile);
  const char* vertexSource = vertexContents.c_str();
  // get fragment shader source code into char array
  std::string fragmentContents = readFile(fragmentFile);
  const char* fragmentSource = fragmentContents.c_str();

  // Apply vertex shader source code to OpenGL shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Apply fragment shader source code to OpenGL shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Link shaders to OpenGL program
  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);
  glLinkProgram(id);

  // Cleanup shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Activate() {
  glUseProgram(id);
}

void Shader::Delete() {
  glDeleteProgram(id);
}
