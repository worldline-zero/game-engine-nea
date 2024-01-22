#version 430

layout (location = 0) in vec2 positions;
layout (location = 1) in vec2 tex_positions;

out vec2 tex_position;
out vec2 position;

void main() {
  gl_Position = vec4(positions, 0.0f, 1.0f);
  position = positions;
  tex_position = tex_positions;
}
