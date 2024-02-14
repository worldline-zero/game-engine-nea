#include "../inc/scene.hpp"

extern struct renderer_state_container renderer_state;

// sdf namespace is because this renderer was originally going to be implemented using ray marching and signed distance functions
namespace sdf {

  void Scene::add_volume(sdf::AABB volume) {
    this->volumes.insert(std::make_pair(volume.ID, volume));
  }

  void Scene::render(Shader &s, Shader &l, glm::vec3 position) const {
    for (const auto &[vol_id, vol] : this->volumes) {
      vol.render(s, l, position);
    }
  }

  void Scene::update() {
    for (auto &[vol_id, vol] : this->volumes) {
      for (auto &[obj_id, obj] : vol.children) {
        obj.position += obj.velocity * renderer_state.frame_time;
        obj.construct_matrix();
      }
    }
  }

}
