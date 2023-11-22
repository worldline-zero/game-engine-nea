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

  /*

  enum object_type_id {
    AABB_id,
    bounding_sphere_id,
    Sphere_id,
    Cuboid_id,
    Arbitrary_id
  };

  class Base_object {

    public:

      glm::mat4 transformation;
      glm::mat4 inverse_scale_trans;
      unsigned int ID;
      object_type_id type;

      glm::vec3 velocity = glm::vec3(0.0f);

      Mesh mesh;

      bool collided = false;

      bool solid = true;

      std::optional<event::timed_job> opt_timed_job = std::nullopt;

      std::function<void(physics::collision_info)> collision_behaviour = [](physics::collision_info c){;};

      float get_scale_factor_svd(glm::mat4 t);

  };

  class Sphere : public Base_object {

    public:

      float radius;
      glm::vec3 center;
      glm::vec3 velocity = glm::vec3(0.0f);

      Sphere();

      Sphere(float r, glm::vec3 c);

      void details();

      float dist(glm::vec3);

      constexpr Sphere &operator=(const Sphere &other) {

        this->radius = other.radius;
        this->center = other.center;

        this->transformation = other.transformation;
        this->inverse_scale_trans = other.inverse_scale_trans;
        this->type = other.type;

        this->mesh = other.mesh;

        return *this;

      }

  };

  class Cuboid : public Base_object {

    public: 

      glm::vec3 half_dimensions;
      glm::vec3 center;
      glm::vec3 rotation;
      float angle;
      glm::vec3 velocity = glm::vec3(0.0f);

      Cuboid();

      Cuboid(glm::vec3 d, glm::vec3 c, glm::vec3 r, float a);

      void details();

      float dist(glm::vec3 pos);

      constexpr Cuboid &operator=(const Cuboid &other) {

        this->half_dimensions = other.half_dimensions;
        this->center = other.center;
        this->rotation = other.rotation;
        this->angle = other.angle;

        this->transformation = other.transformation;
        this->inverse_scale_trans = other.inverse_scale_trans;
        this->type = other.type;

        this->mesh = other.mesh;

        return *this;

      }

  };

  class Arbitrary : public Base_object {
    
    public:

      glm::vec3 scale;
      glm::vec3 center;
      glm::vec3 rotation;
      float angle;
      glm::vec3 velocity = glm::vec3(0.0f);

      Mesh mesh;

      Arbitrary(Mesh m);
      Arbitrary(Mesh m, glm::vec3 s, glm::vec3 p, glm::vec3 r, float a);

      float dist(glm::vec3 pos);

      void details();

      constexpr Arbitrary &operator=(const Arbitrary &other) {

        this->scale = other.scale;
        this->center = other.center;
        this->rotation = other.rotation;
        this->angle = other.angle;
        this->mesh = other.mesh;

        this->transformation = other.transformation;
        this->inverse_scale_trans = other.inverse_scale_trans;
        this->type = other.type;
        
        return *this;

      }

  };
  */

  class Object {
    
    public:

      unsigned int ID;
      
      glm::vec3 position;
      glm::vec3 scale;
      glm::vec3 rotation;
      float angle;
      glm::mat4 transformation;

      sdf::Mesh mesh;

      glm::vec3 velocity;

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
