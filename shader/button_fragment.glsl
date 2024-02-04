#version 430

in vec2 position;
in vec2 texture_coordinate;

out vec4 frag_color;

void main() {
  frag_color = vec4(texture_coordinate.x * 0.7f, texture_coordinate.y * 0.7f, 1.0f, 1.0f);
}
