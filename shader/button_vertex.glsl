#version 430

layout (location = 0) in vec2 b_position;
layout (location = 1) in vec2 texture_coordinates;

out vec2 position;
out vec2 texture_coordinate;

void main() {
  position = b_position;
  texture_coordinate = texture_coordinates;
  gl_Position = vec4(b_position, 0.0f, 1.0f);
}
