#version 330 core

// Values from vertex shader
in vec3 color;
in vec2 texCoords;

// Output variable
out vec4 FragColor;

// Texture uniform from main()
uniform sampler2D tex0;

void main() {
  // Apply texture coordinate/color to pixel
  FragColor = texture(tex0, texCoords);
}