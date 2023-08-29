#ifndef SCENE_H
#define SCENE_H

#include <algorithm>

#include "./bounding.hpp"
#include "../lib/include/glad/gl.h"

namespace sdf {

  struct raycast_info {
    float dist;
    glm::vec3 point;
    bool hit;
    glm::vec3 normal;
  };

  class Scene {

    public:

      using volume_type = std::variant<AABB, bounding_sphere>;
      using volume_container = std::map<unsigned int, volume_type>;

      volume_container volumes;

      std::vector<glm::mat4> transforms;
      std::vector<glm::ivec4> infos; // 4 integers which will represent data about each matrix eg: type of object
                                     // x = type
                                     // y = 
                                     // z = 
                                     // w = 
      std::vector<float> scales;

      unsigned int transformSSBO;
      unsigned int infoSSBO;
      unsigned int scaleSSBO;

      template <class T>
      void add_volume(const T volume) {
        volume_type new_volume = volume;
        volumes.insert(std::make_pair(volume.ID, new_volume));
      }

      void print_volumes(); 

      void print_scene();

      void compile();

      void generate_SSBOs();

      float dist(glm::vec3 pos);

      glm::vec3 normal(glm::vec3 pos);

      raycast_info raycast(glm::vec3 ray_origin, glm::vec3 ray_dir);

      void render(Shader &s);

    private:

      float get_scale_factor_svd(glm::mat4 t);

  };

} // namespace sdf

#endif

