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
  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  
  // Unbind each object before program loop
  vao.Unbind();
  vbo.Unbind();
  ibo.Unbind();



  // Get uniform location from shader program
  GLuint uniID = glGetUniformLocation(shaderProgram.GetID(), "scale");



  // Load in pop cat texture with stb_image
  int imgWidth, imgHeight, imgNumColorChannels;
  std::string imgFilePath = RESOURCES_PATH "textures/pop_cat.png";
  stbi_set_flip_vertically_on_load(true);
  unsigned char* imgBytes = stbi_load(imgFilePath.c_str(), &imgWidth, &imgHeight, &imgNumColorChannels, 0);

  // Assign pop cat texture to OpenGl texture
  GLuint texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBytes);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(imgBytes);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint uniTex0 = glGetUniformLocation(shaderProgram.GetID(), "tex0");
  shaderProgram.Activate();
  glUniform1i(uniTex0, 0);



  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Select shader program
    shaderProgram.Activate();
    // Provide value to shader uniform
    glUniform1f(uniID, 0.5f);

    glBindTexture(GL_TEXTURE_2D, texture);

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
  glDeleteTextures(1, &texture);
  shaderProgram.Delete();

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}