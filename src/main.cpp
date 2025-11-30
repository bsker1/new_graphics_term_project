#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



#define RESOLUTION_X 800
#define RESOLUTION_Y 800



// Define triangle vertices
  GLfloat vertices[] = {
//              COORDINATES                        ||         COLORS
//------------------------------------------------------------------------
//     X   |               Y               |  Z    ||    R   |  G   |  B  |
//------------------------------------------------------------------------
     -0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,       0.8f, 0.3f,  0.02f,  // 0: Lower left
      0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,       0.8f, 0.3f,  0.02f,  // 1: Lower right
      0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,       1.0f, 0.6f,  0.32f,  // 2: Up
    -0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,       0.9f, 0.45f, 0.17f,  // 3: Inner left
     0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,       0.9f, 0.45f, 0.17f,  // 4: Inner right
      0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f,       0.8f, 0.3f,  0.02f   // 5: Inner down
  };

  // Define order to draw triangles
  GLuint indicies[] = {
    0, 5, 3, // Lower left triangle
    5, 1, 4, // Lower right triangle
    3, 4, 2 // Top triangle
  };



int main(void) {
  GLFWwindow* window;

  // Initialize GLFW
  if (!glfwInit()) {
    return -1;
  }

  // Configure GLFW for OpenGL 3.3 core
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Graphics Term Project - Brandon Kiser", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  // Make the window's context current
  glfwMakeContextCurrent(window);

  // Initialize GLAD 
  gladLoadGL();

  // Define OpenGl viewport to size of window
  glViewport(0, 0, RESOLUTION_X, RESOLUTION_Y);



  // Generate shader program from default shader files
  std::string vertexPath = RESOURCES_PATH "shaders/default.vert";
  std::string fragmentPath = RESOURCES_PATH "shaders/default.frag";
  Shader shaderProgram(vertexPath, fragmentPath);



  // Instantiate vao
  VertexArray vao;
  vao.Bind();

  // Instantiate vbo and ibo
  VertexBuffer vbo(vertices, sizeof(vertices));
  IndexBuffer ibo(indicies, sizeof(indicies));

  // Apply vbo configuration to vao
  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  
  vao.Unbind();
  vbo.Unbind();
  ibo.Unbind();



  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Select shader program and vao
    shaderProgram.Activate();
    vao.Bind();

    // Draw triangle
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }



  // Cleanup objects from memory
  vao.Delete();
  vbo.Delete();
  ibo.Delete();
  shaderProgram.Delete();

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}