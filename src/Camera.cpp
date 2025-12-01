#include "Camera.h"

Camera::Camera(const int inWidth, const int inHeight,
  const glm::vec3 inPostition) {
    width = inWidth;
    height = inHeight;
    position = inPostition;
}

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
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speed = 0.1f;
  } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    speed = 0.01f;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += speed * orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position += speed * -orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position += speed * -glm::normalize(
      glm::cross(orientation, upDirection)
    );
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += speed * glm::normalize(
      glm::cross(orientation, upDirection)
    );
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    position += speed * upDirection;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    position += speed * -upDirection;
  }
}