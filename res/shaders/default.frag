#version 330 core

// Values from vertex shader
in vec3 color;
in vec2 texCoords;

// Output variable
out vec4 FragColor;

// Uniforms from main()
uniform sampler2D tex0;
uniform vec4 lightColor;

void main() {
  // Apply texture coordinate/color to pixel
  FragColor = texture(tex0, texCoords) * lightColor;
}