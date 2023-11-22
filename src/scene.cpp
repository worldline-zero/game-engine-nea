#include "../inc/scene.hpp"

extern struct renderer_state_container renderer_state;

namespace sdf {

  void Scene::add_volume(sdf::AABB volume) {
    this->volumes.insert(std::make_pair(volume.ID, volume));
  }

  void Scene::render(Shader &s) const {
    for (const auto &[vol_id, vol] : this->volumes) {
      vol.render(s);
    }
  }

  void Scene::update() {
    for (auto &[vol_id, vol] : this->volumes) {
      for (auto &[obj_id, obj] : vol.children) {
        obj.transformation = glm::translate(obj.transformation, obj.velocity * renderer_state.frame_time);
      }
    }
  }

}
