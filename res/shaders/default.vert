#version 330 core

// Read in data from vertex buffer to respective vectors
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

// Define variables to be sent to fragment shader
out vec3 color;
out vec2 texCoords;
out vec3 normal;
out vec3 currPos;

// Provided transformation matrix
uniform mat4 camMatrix;
uniform mat4 model;

void main() {
  currPos = vec3(model * vec4(aPos, 1.0f));

  // Apply transformation matrix to vertex position
  gl_Position = camMatrix * vec4(currPos, 1.0);
  // Assign variables for fragment shader
  color = aColor;
  texCoords = aTex;
  normal = aNormal;
}