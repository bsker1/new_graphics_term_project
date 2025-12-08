#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Shader.h"

class Camera {
  private:
    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    bool firstClick = true;

    int width;
    int height;

    float speed = 0.01f;
    float sensitivity = 100.0f;

  public:
    Camera(const int inWidth, const int inHeight, const glm::vec3 inPostition);
    
    glm::vec3 GetPosition() { return position; }
    
    void UpdateMatrix(const float yFovDegree, const float aspect, const float nearPlane, const float farPlane);
    void SetMatrix(Shader& shaderProgram, const char* uniform);
    void Inputs(GLFWwindow* window);
};

#endif