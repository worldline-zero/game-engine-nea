#version 430

layout (location = 0) in vec2 b_position;

out vec2 position;

void main() {
  position = b_position;
  gl_Position = vec4(b_position, 0.0f, 1.0f);
}
