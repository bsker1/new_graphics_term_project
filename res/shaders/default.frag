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

vec4 directionalLight() {
  vec3 normalizedNormal = normalize(normal);
  vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));

  float diffuse = max(dot(normalizedNormal, lightDirection), 0.0f);
  float ambient = 0.2f;

  float maxSpecularLight = 0.5f;
  vec3 viewDirection = normalize(camPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * maxSpecularLight;

  return (texture(tex0, texCoords) * (diffuse + ambient) + texture(tex1, texCoords).r * specular) * lightColor;
}

vec4 spotLight() {
  float cosOuterCone = 0.90f;
  float cosInnerCone = 0.95f;

  vec3 normalizedNormal = normalize(normal);
  vec3 lightDirection = normalize(lightPos - currPos);

  float diffuse = max(dot(normalizedNormal, lightDirection), 0.0f);
  float ambient = 0.2f;

  float maxSpecularLight = 0.5f;
  vec3 viewDirection = normalize(camPos - currPos);
  vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
  float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
  float specular = specAmount * maxSpecularLight;

  float lightAngle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
  float intensity = clamp((lightAngle - cosOuterCone) / (cosInnerCone - cosOuterCone), 0.0f, 1.0f);

  return (texture(tex0, texCoords) * (diffuse * intensity + ambient) + texture(tex1, texCoords).r * specular * intensity) * lightColor;
}

void main() {
  FragColor = spotLight();
}