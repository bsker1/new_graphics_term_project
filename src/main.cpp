#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "stb/stb_image.h"

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"



#define RESOLUTION_X 800
#define RESOLUTION_Y 800



// Define square vertices
  GLfloat vertices[] = {
//      COORDINATES       ||       COLORS       ||   TEXTURE COORDINATES  
//------------------------||--------------------||-------------------------
//    X   |  Y  |  Z      ||    R  |  G  | B    ||   X  |  Y
    -0.5f, -0.5f, 0.0f,       1.0f, 0.0f, 0.0f,     0.0f, 0.0f, // 0: Lower left corner
     0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f,     1.0f, 0.0f, // 1: Lower right corner
     0.5f,  0.5f, 0.0f,       0.0f, 0.0f, 1.0f,     1.0f, 1.0f, // 2: Upper right corner
    -0.5f,  0.5f, 0.0f,       1.0f, 1.0f, 1.0f,     0.0f, 1.0f  // 3: Upper left corner
  };

  // Define order to draw triangles
  GLuint indicies[] = {
    0, 1, 2, // Lower right triangle
    2, 3, 0  // Upper left triangle
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
  window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y,
    "Graphics Term Project - Brandon Kiser", NULL, NULL);
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
  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float),
    (void*)(3 * sizeof(float)));
  vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float),
    (void*)(6 * sizeof(float)));
  
  // Unbind each object before program loop
  vao.Unbind();
  vbo.Unbind();
  ibo.Unbind();



  // Load pop cat image into OpenGL texture
  std::string imgFilePath = RESOURCES_PATH "textures/pop_cat.png";
  Texture popCat(imgFilePath, GL_TEXTURE_2D, 0, GL_RGBA);
  // Apply value to tex0 uniform in fragment shader
  popCat.TexUnit(shaderProgram, "tex0", 0);



  // Get uniform location from shader program
  GLuint uniID = glGetUniformLocation(shaderProgram.GetID(), "scale");



  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Select shader program
    shaderProgram.Activate();
    // Provide value to shader uniform
    glUniform1f(uniID, 0.5f);

    // Bind texture with pop cat image
    popCat.Bind();

    // Bind vao with triangle vertex/index values
    vao.Bind();

    // Draw triangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }



  // Cleanup objects from memory
  vao.Delete();
  vbo.Delete();
  ibo.Delete();
  popCat.Delete();
  shaderProgram.Delete();

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}