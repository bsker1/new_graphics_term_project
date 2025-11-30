#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <cmath>
#include <iostream>
#include <string>
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



int main(void) {
  GLFWwindow* window;

  // Initialize GLFW
  if (!glfwInit()) {
    return -1;
  }

  // Configure GLFW for OpenGL 3.3 core
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(800, 800, "Graphics Term Project - Brandon Kiser", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLAD 
  gladLoadGL();



  // Apply vertex shader source code to OpenGL shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::string vertexShaderSource = readFile(RESOURCES_PATH "shaders/Vertex.shader");
  const char* vertexSrc = vertexShaderSource.c_str();
  std::cout << "{start}" << vertexShaderSource << "{end}" << std::endl;
  glShaderSource(vertexShader, 1, &vertexSrc, NULL);
  glCompileShader(vertexShader);

  // Apply fragment shader source code to OpenGL shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragmentShaderSource = readFile(RESOURCES_PATH "shaders/Fragment.shader");
  const char* fragmentSrc = fragmentShaderSource.c_str();
  std::cout << "{start}" << fragmentShaderSource << "{end}" << std::endl;
  glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
  glCompileShader(fragmentShader);

  // Link shaders to OpenGL program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Cleanup shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);



  // Define triangle vertices
  GLfloat vertices[] = {
    -0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,
     0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,
     0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f
  };



  // Configure vertex array and vertex buffer objects
  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  // Bind objects
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Apply verticies array to vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Define layout of vbo data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Unbind objects for cleanup
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);



  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Select shader program and vao
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    // Draw triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }



  // Cleanup objects from memory
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(shaderProgram);

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}