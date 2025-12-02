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
#define FPS 60



// Define pyramid vertices
GLfloat vertices[] = {
  -0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,      0.0f, 0.0f,
   0.5f, 0.0f,  0.5f,    0.83f, 0.70f, 0.44f,      5.0f, 0.0f,
   0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,      0.0f, 0.0f,
  -0.5f, 0.0f, -0.5f,    0.83f, 0.70f, 0.44f,      5.0f, 0.0f,
   0.0f, 0.8f,  0.0f,    0.92f, 0.86f, 0.76f,      2.5f, 5.0f
};

// Define order to draw triangles
GLuint indices[] = {
  0, 1, 2,
  2, 3, 0,
  0, 1, 4,
  1, 2, 4,
  2, 3, 4,
  3, 0, 4
};



GLfloat lightVertices[] = {
  -0.1f, -0.1f,  0.1f,
   0.1f, -0.1f,  0.1f,
   0.1f, -0.1f, -0.1f,
  -0.1f, -0.1f, -0.1f,
  -0.1f,  0.1f,  0.1f,
   0.1f,  0.1f,  0.1f,
   0.1f,  0.1f, -0.1f,
  -0.1f,  0.1f, -0.1f,
};

GLuint lightIndices[] = {
  0, 1, 2,
  2, 3, 0,
  0, 1, 5,
  5, 4, 0,
  1, 2, 6,
  6, 5, 1,
  2, 3, 7,
  7, 6, 2,
  3, 0, 4,
  4, 7, 3,
  4, 5, 6,
  6, 7, 4
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

  // Create vbo and ibo, load with vertices and indices respectively
  VertexBuffer vbo(vertices, sizeof(vertices));
  IndexBuffer ibo(indices, sizeof(indices));

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



  std::string lightVertexPath = RESOURCES_PATH "shaders/light.vert";
  std::string lightFragmentPath = RESOURCES_PATH "shaders/light.frag";
  Shader lightShaderProgram(lightVertexPath, lightFragmentPath);

  VertexArray lightVAO;
  lightVAO.Bind();

  VertexBuffer lightVBO(lightVertices, sizeof(lightVertices));
  IndexBuffer lightIBO(lightIndices, sizeof(lightIndices));

  lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(GL_FLOAT),
    (void*)0);
  
  lightVAO.Unbind();
  lightVBO.Unbind();
  lightIBO.Unbind();



  glm::vec4 lightColor = glm::vec4(1.0f, 0.2f, 0.5f, 1.0f);

  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  glm::vec3 pyramidPos = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::mat4 pyramidModel = glm::mat4(1.0f);
  pyramidModel = glm::translate(pyramidModel, pyramidPos);



  lightShaderProgram.Activate();
  GLuint uniLightModel = glGetUniformLocation(lightShaderProgram.GetID(),
    "model");
  glUniformMatrix4fv(uniLightModel, 1, GL_FALSE,
    glm::value_ptr(lightModel));
  GLuint uniLightColor = glGetUniformLocation(lightShaderProgram.GetID(),
    "lightColor");
  glUniform4f(uniLightColor, lightColor.x, lightColor.y, lightColor.z,
    lightColor.w);

  shaderProgram.Activate();
  GLuint uniPyramidModel = glGetUniformLocation(shaderProgram.GetID(),
    "model");
  glUniformMatrix4fv(uniPyramidModel, 1, GL_FALSE,
    glm::value_ptr(pyramidModel));
  GLuint uniPyramidLightColor = glGetUniformLocation(shaderProgram.GetID(),
    "lightColor");
  glUniform4f(uniPyramidLightColor, lightColor.x, lightColor.y, lightColor.z,
    lightColor.w);


  // Create camera object with screen resolution and starting position
  Camera camera(RESOLUTION_X, RESOLUTION_Y, glm::vec3(0.0f, 0.0f, 2.0f));

  // Only print unobstructed triangles
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Inputs(window);
    
    camera.UpdateMatrix(45.0f, RESOLUTION_X / (float)RESOLUTION_Y,
      0.1f, 100.0f);

    // Select shader program
    shaderProgram.Activate();
    camera.SetMatrix(shaderProgram, "camMatrix");

    // Bind brick texture
    brick.Bind();

    // Bind vao with vertex/index values
    vao.Bind();

    // Draw pyramid
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint),
      GL_UNSIGNED_INT, 0);
    
    lightShaderProgram.Activate();
    camera.SetMatrix(lightShaderProgram, "camMatrix");
    lightVAO.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint),
      GL_UNSIGNED_INT, 0);

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