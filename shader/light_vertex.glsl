#version 430 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texture_coordinate;

out vec3 fragment_position;
out vec3 normal;
out vec2 texture_coordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  fragment_position = vec3(model * vec4(a_position, 1.0f));
  texture_coordinate = a_texture_coordinate;
  normal = a_normal;
  gl_Position = projection * view * vec4(fragment_position, 1.0f);
}
