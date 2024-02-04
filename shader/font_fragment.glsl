#version 430

in vec2 tex_position;
in vec2 position;

out vec4 frag_color;

uniform sampler2D bitmap_font;
uniform vec4 font_color;

void main() {
  vec4 tex_color = texture(bitmap_font, tex_position);
  if (tex_color.a < 0.01f) {
    discard;
  }
  frag_color = font_color;
}
