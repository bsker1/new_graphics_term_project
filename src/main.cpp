#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Camera.h"



#define RESOLUTION_X 800
#define RESOLUTION_Y 800
#define FPS 144



// Define pyramid vertices
GLfloat vertices[] = {
  -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,      0.0f, 0.0f,
   0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,      5.0f, 0.0f,
   0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,      0.0f, 0.0f,
  -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,      5.0f, 0.0f,
   0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,      2.5f, 5.0f
};

// Define order to draw triangles
GLuint indicies[] = {
  0, 1, 2,
  2, 3, 0,
  0, 1, 4,
  1, 2, 4,
  2, 3, 4,
  3, 0, 4
};



int main(void) {
  // Create GLFW windo object
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



  // Create vao
  VertexArray vao;
  vao.Bind();

  // Create vbo and ibo, load with vertices and indicies respectively
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



  // Load brick image into OpenGL texture
  std::string imgFilePath = RESOURCES_PATH "textures/brick.png";
  Texture brick(imgFilePath, GL_TEXTURE_2D, 0, GL_RGBA);
  // Apply value to tex0 uniform in fragment shader
  brick.TexUnit(shaderProgram, "tex0", 0);



  // Create pyramid rotation variables
  float rotation = 0.0f;
  double prevTime = glfwGetTime();

  // Only print unobstructed triangles
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select shader program
    shaderProgram.Activate();

    // Create transformation matrices, initialize to fill with 1.0
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    // Iterate rotation by 0.5 degrees each frame
    double currTime = glfwGetTime();
    if (currTime - prevTime >= 1 / (double)FPS) {
      rotation += 0.5f;
      prevTime = currTime;
    }
    // Apply rotation on y-axis
    model = glm::rotate(model, glm::radians(rotation),
      glm::vec3(0.0f, 1.0f, 0.0f));
    // Move world down 0.5 and back 2.0
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
    // Apply perspective yfov = 45 degrees, window aspect ratio,
    // near clipping plane 0.1, far clipping plane 100.0
    proj = glm::perspective(glm::radians(45.0f),
      (float)RESOLUTION_X / RESOLUTION_Y, 0.1f, 100.0f);
    
    // Load matrix uniforms in shader with calculated matrices
    GLuint uniModel = glGetUniformLocation(shaderProgram.GetID(), "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
    GLuint uniView = glGetUniformLocation(shaderProgram.GetID(), "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
    GLuint uniProj = glGetUniformLocation(shaderProgram.GetID(), "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    // Bind brick texture
    brick.Bind();

    // Bind vao with vertex/index values
    vao.Bind();

    // Draw pyramid
    glDrawElements(GL_TRIANGLES, sizeof(indicies) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }



  // Cleanup objects from memory
  vao.Delete();
  vbo.Delete();
  ibo.Delete();
  brick.Delete();
  shaderProgram.Delete();

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}