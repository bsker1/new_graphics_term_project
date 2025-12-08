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

vec4 pointLight() {
  vec3 lightVec = lightPos - currPos;
  float lightDistance = length(lightVec);
  float quadratic = 1.0f;
  float linear = 0.04f;
  float intensity = 1.0f / (quadratic * pow(lightDistance, 2) + linear * lightDistance + 1.0f);

  vec3 normalizedNormal = normalize(normal);
  vec3 lightDirection = normalize(lightVec);

  float diffuse = max(dot(normalizedNormal, lightDirection), 0.0f);
  float ambient = 0.2f;

  float maxSpecularLight = 0.5f;
  vec3 viewDirection = normalize(camPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * maxSpecularLight;

  return (texture(tex0, texCoords) * (diffuse * intensity + ambient) + texture(tex1, texCoords).r * specular * intensity) * lightColor;
}

void main() {
  FragColor = pointLight();
}