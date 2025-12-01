#include "Camera.h"

void Camera::Matrix(const float yFovDegree, const float aspect,
  const float nearPlane, const float farPlane, Shader& shaderProgram,
  const char* uniform) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(position, position + orientation, upDirection);
    proj = glm::perspective(glm::radians(yFovDegree), aspect, nearPlane,
      farPlane);
    
    GLuint uniID = glGetUniformLocation(shaderProgram.GetID(), uniform);
    glUniformMatrix4fv(uniID, 1, GL_FALSE, glm::value_ptr(proj * view));
}

void Camera::Inputs(GLFWwindow* window) {
  return;
}