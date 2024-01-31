#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <functional>
#include <optional>

#include "./mesh.hpp"

#include "./shader.hpp"

#include "./timed_job.hpp"

#define EPSILON 0.001f

namespace sdf {

  class Object {
    
    public:

      unsigned int ID;
      
      glm::vec3 position;
      glm::vec3 scale;
      glm::vec3 rotation;
      float angle;
      glm::mat4 transformation;

      sdf::Mesh mesh;
      glm::vec4 color;
      float light_intensity;


      glm::vec3 velocity;
      glm::vec3 acceleration;
      float rotational_velocity;

      bool collided = false;

      bool solid = true;

      std::optional<event::timed_job> opt_timed_job = std::nullopt;

      std::function<void(physics::collision_info)> collision_behaviour = [](physics::collision_info c){;};

      Object(sdf::Mesh m, glm::vec3 p, glm::vec3 s, glm::vec3 r, float a);

      void construct_matrix();

  };


  struct Capsule {
    glm::vec3 base;
    glm::vec3 tip;
    float radius;
    
    Capsule();
    Capsule(glm::vec3 b, glm::vec3 t, float r);

    constexpr Capsule &operator=(const Capsule &other) {

      this->base = other.base;
      this->tip = other.tip;
      this->radius = other.radius;

      return *this;

    }
  };

} // namespace sdf

#endif
