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
  std::string vertexShaderSource = readFile(RESOURCES_PATH "shaders/default.vert");
  const char* vertexSrc = vertexShaderSource.c_str();
  glShaderSource(vertexShader, 1, &vertexSrc, NULL);
  glCompileShader(vertexShader);

  // Apply fragment shader source code to OpenGL shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  std::string fragmentShaderSource = readFile(RESOURCES_PATH "shaders/default.frag");
  const char* fragmentSrc = fragmentShaderSource.c_str();
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
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // 0: Lower left
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // 1: Lower right
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // 2: Up
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // 3: Inner left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // 4: Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // 5: Inner down
  };

  // Define order to draw triangles
  GLuint indicies[] = {
    0, 5, 3, // Lower left triangle
    5, 1, 4, // Lower right triangle
    3, 4, 2 // Top triangle
  };



  // Configure vertex array, vertex buffer, and index buffer objects
  GLuint vao, vbo, ibo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ibo);

  // Bind vao
  glBindVertexArray(vao);

  // Bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Apply verticies array to vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind ibo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  // Apply indicies array to ibo
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

  // Define layout of vbo data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Unbind objects for cleanup
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Select shader program and vao
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    // Draw triangle
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }



  // Cleanup objects from memory
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteProgram(shaderProgram);

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}