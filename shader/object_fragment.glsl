#version 430 core

struct Light {
  vec4 color_and_intensity;
  vec4 position;
};

#define AMBIENT_STRENGTH 0.10f
#define SPECULAR_STRENGTH 0.5f

#define LINEAR_ATTENUATION_COMPONENT 0.014f
#define QUADRATIC_ATTENUATION_COMPONENT 0.0007f

layout (std430, binding = 4) buffer scene_lights {
  Light lights[];
};

out vec4 fragment_color;

in vec3 fragment_position;
in vec3 normal;
in vec2 texture_coordinate;

uniform vec4 input_color;
uniform vec3 view_position;

vec3 point_light(Light light, vec3 view_direction) {

  vec3 light_direction = normalize(light.position.xyz - fragment_position);
  vec3 diffuse = max(dot(light_direction, normalize(normal)), 0.0f) * light.color_and_intensity.xyz;

  vec3 halfway_vector = normalize(light_direction + view_direction);
  vec3 reflect_direction = reflect(-light_direction, normal);
  vec3 specular = pow(max(dot(normal, halfway_vector), 0.0f), 256) * SPECULAR_STRENGTH * light.color_and_intensity.xyz;

  float distance_to_fragment = length(light.position.xyz - fragment_position);
  float attenuation = 1.0f / 
    (1.0 +
     LINEAR_ATTENUATION_COMPONENT * distance_to_fragment + 
     QUADRATIC_ATTENUATION_COMPONENT * (distance_to_fragment * distance_to_fragment)
    );
  
  vec3 result = (attenuation * (diffuse + specular)) * vec3(input_color) * light.color_and_intensity.w;

  return result;

}

void main() {

  vec3 view_direction = normalize(view_position - fragment_position);
  
  vec3 result = vec3(0.0f);

  for (int i = 0; i<lights.length(); i++) {
    result += point_light(lights[i], view_direction);
  }

  result += AMBIENT_STRENGTH * vec3(input_color);

  fragment_color = vec4(result, input_color.w);
}
