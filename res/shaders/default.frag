#version 330 core

// Values from vertex shader
in vec3 color;
in vec2 texCoords;
in vec3 normal;
in vec3 currPos;

// Output variable
out vec4 FragColor;

// Uniforms from main()
uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
  vec3 normalizedNormal = normalize(normal);
  vec3 lightDirection = normalize(lightPos - currPos);

  float diffuse = max(dot(normalizedNormal, lightDirection), 0.0f);
  float ambient = 0.2f;

  float maxSpecularLight = 0.5f;
  vec3 viewDirection = normalize(camPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * maxSpecularLight;

  // Apply texture coordinate/color to pixel
  FragColor = texture(tex0, texCoords) * lightColor * (diffuse + ambient) + texture(tex1, texCoords).r * specular;
}