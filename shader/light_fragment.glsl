#version 430

out vec4 fragment_color;

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coordinate;

uniform vec4 input_color;
uniform mat4 model;

void main() {
  /*
  vec3 norm = normalize(Normal);
  float r = dot(norm, vec3(1.0f, 0.0f, 0.0f));
  float g = dot(norm, vec3(0.0f, 1.0f, 0.0f));
  float b = dot(norm, vec3(0.0f, 0.0f, 1.0f));
  FragColor = vec4(vec3((r+1.0f)/2.0f, (g+1.0f)/2.0f, (b+1.0f)/2.0f), 1.0f);
  */
  float distance_to_fragment = length(fragment_position - vec3(model * vec4(vec3(0.0f), 1.0f)));
  //fragment_color = input_color / vec4(vec3(1.0f), 0.0f) + (vec4(fragment_position, 1.0f) - (model * vec4(0.0f, 0.0f, 0.0f, 1.0f)));
  //fragment_color = input_color / vec4(vec3(1.0f), 0.0f) + vec4(vec3(distance_to_fragment), 1.0f);
  fragment_color = mix(input_color, vec4(1.0f) / (vec4(vec3(1.0f), 0.0f) + vec4(vec3(distance_to_fragment), 1.0f)), 0.4f);
}
