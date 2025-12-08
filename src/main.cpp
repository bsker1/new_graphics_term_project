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



// Define floor vertices
GLfloat vertices[] = {
//     COORDINATES    /      COLORS       /  TexCoord   /      NORMALS     //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Define order to draw triangles
GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};



// Define light cube vertices
GLfloat lightVertices[] = {
  //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

// Define order to draw triangles
GLuint lightIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
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

  // Create vao
  VertexArray vao;
  vao.Bind();

  // Create vbo and ibo, load with vertices and indices respectively
  VertexBuffer vbo(vertices, sizeof(vertices));
  IndexBuffer ibo(indices, sizeof(indices));

  // Apply vbo configuration to vao
  vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
  vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
  vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
  vao.LinkAttrib(vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
  
  // Unbind each object before program loop
  vao.Unbind();
  vbo.Unbind();
  ibo.Unbind();

  // Load planks image into OpenGL texture
  std::string texFilePath = RESOURCES_PATH "textures/planks.png";
  Texture planksTex(texFilePath, GL_TEXTURE_2D, 0, GL_RGBA);
  // Apply value to tex0 uniform in fragment shader
  planksTex.TexUnit(shaderProgram, "tex0", 0);
  std::string specMapFilePath = RESOURCES_PATH "textures/planksSpec.png";
  Texture planksSpec(specMapFilePath, GL_TEXTURE_2D, 1, GL_RED);
  planksSpec.TexUnit(shaderProgram, "tex1", 1);




  // Generate shader program from light shader files
  std::string lightVertexPath = RESOURCES_PATH "shaders/light.vert";
  std::string lightFragmentPath = RESOURCES_PATH "shaders/light.frag";
  Shader lightShaderProgram(lightVertexPath, lightFragmentPath);

  // Create vao
  VertexArray lightVAO;
  lightVAO.Bind();

  // Create vbo and ibo, load with light vertices and indices respectively
  VertexBuffer lightVBO(lightVertices, sizeof(lightVertices));
  IndexBuffer lightIBO(lightIndices, sizeof(lightIndices));

  // Apply vbo configuration to vao
  lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(GL_FLOAT), (void*)0);
  
  // Unbind each object before program loop
  lightVAO.Unbind();
  lightVBO.Unbind();
  lightIBO.Unbind();



  // Define light color
  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  // Define light cube position
  glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
  // Translate light cube model by position vector
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  // Define floor position
  glm::vec3 floorPos = glm::vec3(0.0f, 0.0f, 0.0f);
  // Translate floor model by position vector
  glm::mat4 floorModel = glm::mat4(1.0f);
  floorModel = glm::translate(floorModel, floorPos);



  // Apply light model and color uniforms to light shader
  lightShaderProgram.Activate();
  GLuint uniLightModel = glGetUniformLocation(lightShaderProgram.GetID(), "model");
  glUniformMatrix4fv(uniLightModel, 1, GL_FALSE, glm::value_ptr(lightModel));
  GLuint uniLightColor = glGetUniformLocation(lightShaderProgram.GetID(), "lightColor");
  glUniform4f(uniLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);

  // Apply model, light color, and light position uniforms to default shader
  shaderProgram.Activate();
  GLuint uniPyramidModel = glGetUniformLocation(shaderProgram.GetID(), "model");
  glUniformMatrix4fv(uniPyramidModel, 1, GL_FALSE, glm::value_ptr(floorModel));
  GLuint uniPyramidLightColor = glGetUniformLocation(shaderProgram.GetID(), "lightColor");
  glUniform4f(uniPyramidLightColor, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  GLuint uniPyramidLightPos = glGetUniformLocation(shaderProgram.GetID(), "lightPos");
  glUniform3f(uniPyramidLightPos, lightPos.x, lightPos.y, lightPos.z);


  // Create camera object with screen resolution and starting position
  Camera camera(RESOLUTION_X, RESOLUTION_Y, glm::vec3(0.0f, 0.0f, 2.0f));

  // Only print unobstructed triangles
  glEnable(GL_DEPTH_TEST);

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window)) {
    // Clear screen to navy blue
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get user input for camera movement
    camera.Inputs(window);
    
    // Apply view and projection matrices to camera for realistic 3D
    camera.UpdateMatrix(45.0f, RESOLUTION_X / (float)RESOLUTION_Y,
      0.1f, 100.0f);

    // Select default shader program
    shaderProgram.Activate();
    // Apply camera position and view/projection matrices to shader uniforms
    GLuint uniCamPos = glGetUniformLocation(shaderProgram.GetID(), "camPos");
    glUniform3f(uniCamPos, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    camera.SetMatrix(shaderProgram, "camMatrix");

    // Bind planks texture
    planksTex.Bind();
    planksSpec.Bind();

    // Bind vao with vertex/index values
    vao.Bind();

    // Draw floor
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
    // Select light shader program
    lightShaderProgram.Activate();
    // Apply camera position and view/projection matrices to shader uniforms
    camera.SetMatrix(lightShaderProgram, "camMatrix");
    lightVAO.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }



  // Cleanup objects from memory
  vao.Delete();
  vbo.Delete();
  ibo.Delete();
  planksTex.Delete();
  planksSpec.Delete();
  shaderProgram.Delete();

  // Cleanup GLFW window object
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}