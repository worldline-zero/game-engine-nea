#include "../inc/scene.hpp"

namespace sdf {

  void Scene::print_volumes() {
    std::cout << "\n";
    for (const auto &[id_, vol_]:this->volumes) {
      std::visit([](auto v){v.details();std::cout << "\n";}, vol_);
    }
  }

  void Scene::print_scene() {
    std::cout << "\n";
    for (const auto &[id_, vol_]:this->volumes) {
      std::visit([](auto v){v.details(); v.print_children(); std::cout << "\n";}, vol_);
    }
  }

  void Scene::compile() {

    this->transforms.clear();
    this->infos.clear();
    this->scales.clear();

    for (const auto &[vol_id, vol]:volumes) {
      std::visit([this] (auto v) mutable {
        this->transforms.push_back(v.transformation);
        this->infos.push_back(glm::ivec4(v.type, 0, 0, 0));
        this->scales.push_back(get_scale_factor_svd(v.transformation));
        for (const auto &[obj_id, obj]:v.children) {
          std::visit([this] (auto o) mutable {
            this->transforms.push_back(o.transformation);
            this->infos.push_back(glm::ivec4(o.type, 0, 0, 0));
            this->scales.push_back(get_scale_factor_svd(o.transformation));
          }, obj);
        }
      }, vol);
    }

    generate_SSBOs();

  }

  void Scene::generate_SSBOs() {
    
    unsigned int transform_ssbo, info_ssbo, scale_ssbo;
    glGenBuffers(1, &transform_ssbo);
    glGenBuffers(1, &info_ssbo);
    glGenBuffers(1, &scale_ssbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, transform_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->transforms.size() * sizeof(glm::mat4), &this->transforms[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, transform_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind buffer now that im done (for clarity)

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, info_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->infos.size() * sizeof(glm::ivec4), &this->infos[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, info_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, scale_ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->scales.size() * sizeof(float), &this->scales[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, scale_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    this->transformSSBO = transform_ssbo;
    this->infoSSBO = info_ssbo;
    this->scaleSSBO = scale_ssbo;

  }

  float Scene::dist(glm::vec3 pos) {
    float min_dist = 2147483647.0f;
    float vol_dist = 2147483647.0f;
    bool box_intersect = false;
    for (const auto &[vol_id, vol] : this->volumes) {
      std::visit([&vol_dist, pos](auto v) { vol_dist = std::min(v.dist(pos), vol_dist); }, vol);
      if (vol_dist <= EPSILON*5.0f) {
        std::visit([&min_dist, pos, &box_intersect](auto v) mutable {
          box_intersect = true;
          for (const auto &[obj_id, obj] : v.children) {
            std::visit([&min_dist, pos](auto o) mutable {
              min_dist = std::min(min_dist, o.dist(pos));
            }, obj);
          }
        }, vol);
      }
    }
    if (box_intersect) {
      return min_dist;
    } else {
      return vol_dist;
    }
  }

  glm::vec3 Scene::normal(glm::vec3 pos) {
    glm::vec3 v1 = glm::vec3(
        std::abs(this->dist(pos + glm::vec3(EPSILON, 0.0f, 0.0f))),
        std::abs(this->dist(pos + glm::vec3(0.0f, EPSILON, 0.0f))),
        std::abs(this->dist(pos + glm::vec3(0.0f, 0.0f, EPSILON))));
    glm::vec3 v2 = glm::vec3(
        std::abs(this->dist(pos - glm::vec3(EPSILON, 0.0f, 0.0f))),
        std::abs(this->dist(pos - glm::vec3(0.0f, EPSILON, 0.0f))),
        std::abs(this->dist(pos - glm::vec3(0.0f, 0.0f, EPSILON))));
    return glm::normalize(v1 - v2);
  }

  raycast_info Scene::raycast(glm::vec3 ray_origin, glm::vec3 ray_dir) {
    float safe_dist;
    float total_dist = 0.0f;
    float max_steps = 255.0f;
    float max_dist = 1000.0f;
    glm::vec3 new_pos = ray_origin;
    for (float i = 0.0f; i<max_steps && total_dist<max_dist; i+=1.0f) {
      safe_dist = this->dist(new_pos);
      if (safe_dist < EPSILON) {
        raycast_info result = {
          .dist = total_dist,
          .point = new_pos,
          .hit = true,
          .normal = this->normal(new_pos)
        };
        return result;
      } else {
        total_dist += safe_dist;
        new_pos = ray_origin + (total_dist * ray_dir);
      }
    }
    raycast_info result = {
      .dist = total_dist,
      .point = new_pos,
      .hit = false,
      .normal = glm::vec3(1.0f)
    };
    return result;
  }

  void Scene::render(Shader &s) {
    for (const auto &[vol_id, vol] : this->volumes) {
      std::visit([&s](auto v) { v.render(s); }, vol);
    }
  }

  float Scene::get_scale_factor_svd(glm::mat4 t) {
    t = glm::inverse(t);
    float sx = glm::length(glm::vec3(t[0][0], t[0][1], t[0][2]));
    float sy = glm::length(glm::vec3(t[1][0], t[1][1], t[1][2]));
    float sz = glm::length(glm::vec3(t[2][0], t[2][1], t[2][2]));
    return std::min(sx, std::min(sy, sz));
  }


}
