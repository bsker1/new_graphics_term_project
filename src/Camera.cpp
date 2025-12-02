#include "Camera.h"

Camera::Camera(const int inWidth, const int inHeight,
  const glm::vec3 inPostition) {
    width = inWidth;
    height = inHeight;
    position = inPostition;
}

void Camera::UpdateMatrix(const float yFovDegree, const float aspect,
  const float nearPlane, const float farPlane) {
    // Initialize view and projection matrices
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    // Define view matrix with positional vectors
    view = glm::lookAt(position, position + orientation, upDirection);
    // Define projection matrix with provided worldview properties
    proj = glm::perspective(glm::radians(yFovDegree), aspect, nearPlane,
      farPlane);
    
    cameraMatrix = proj * view;
}

void Camera::SetMatrix(Shader& shaderProgram, const char* uniform) {
    // Apply projection and view matrix to provided uniform for
    // vertex shader
    GLuint uniID = glGetUniformLocation(shaderProgram.GetID(), uniform);
    glUniformMatrix4fv(uniID, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
  // Hold shift to control movement speed
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    speed = 0.1f;
  }
  else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    speed = 0.01f;
  }

  // WASD to move forward/back, strafe left/right
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
  // Space/ctrl to move up/down respectively
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    position += speed * upDirection;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    position += speed * -upDirection;
  }

  // Check if left mouse button is pressed
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // Hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Set cursor to middle of screen on click
    if (firstClick) {
      glfwSetCursorPos(window, width / 2, height / 2);
      firstClick = false;
    }

    // Get cursor coordinates
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Set rotation angles based on mouse movement
    float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
    float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

    // Set new orientation to mouse rotation on current orientation
    glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX),
      glm::normalize(glm::cross(orientation, upDirection)));
    
    // Apply rotX rotation if within 5 degrees
    if (!((glm::angle(newOrientation, upDirection) <= glm::radians(5.0f)) or
      (glm::angle(newOrientation, -upDirection) <= glm::radians(5.0f)))) {
        orientation = newOrientation;
    }

    // Apply rotY rotation
    orientation = glm::rotate(orientation, glm::radians(-rotY), upDirection);

    // Reposition cursor to middle of screen
    glfwSetCursorPos(window, width / 2, height / 2);
  }
  else if (glfwGetMouseButton(window,
    GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
      // Show cursor on release
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      firstClick = true;
  }
}