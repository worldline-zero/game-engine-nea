#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <type_traits>

#include "./bounding.hpp"
#include "./renderer_state.hpp"
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
      //using volume_child_pair = std::pair<std::optional<volume_type>&, std::optional<bounding_volume::child_type>&>;

      volume_container volumes;

      // legacy code

      std::vector<glm::mat4> transforms;
      std::vector<glm::ivec4> infos; 
      std::vector<float> scales;

      unsigned int transformSSBO;
      unsigned int infoSSBO;
      unsigned int scaleSSBO;

      // legacy code end

      template <class T>
      void add_volume(const T volume) {
        volume_type new_volume = volume;
        volumes.insert(std::make_pair(volume.ID, new_volume));
      }

      void print_volumes(); 

      void print_scene();

      // legacy code 

      void compile();

      void generate_SSBOs();

      const float dist(glm::vec3 pos) const;

      glm::vec3 normal(glm::vec3 pos);

      raycast_info raycast(glm::vec3 ray_origin, glm::vec3 ray_dir);

      //legacy code end

      void render(Shader &s);

      void update();

      volume_type &operator[](const unsigned int id) {
        return volumes.at(id);
      } 

    private:

      float get_scale_factor_svd(glm::mat4 t);

  };

} // namespace sdf

#endif

