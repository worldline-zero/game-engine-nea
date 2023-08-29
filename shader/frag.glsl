#version 430 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

void main() {
  vec3 norm = normalize(Normal);
  float r = dot(norm, vec3(1.0f, 0.0f, 0.0f));
  float g = dot(norm, vec3(0.0f, 1.0f, 0.0f));
  float b = dot(norm, vec3(0.0f, 0.0f, 1.0f));
  FragColor = vec4(vec3((r+1.0f)/2.0f, (g+1.0f)/2.0f, (b+1.0f)/2.0f), 1.0f);
}
