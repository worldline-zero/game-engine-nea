#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "./mesh.hpp"

#include "./shader.hpp"

#define EPSILON 0.001f

namespace sdf {

  enum object_type_id {
    AABB_id,
    bounding_sphere_id,
    Sphere_id,
    Cuboid_id
  };

  class Base_object {

    public:

      glm::mat4 transformation;
      glm::mat4 inverse_scale_trans;
      unsigned int ID;
      object_type_id type;

      Mesh mesh;

      float get_scale_factor_svd(glm::mat4 t);

  };

  class Sphere : public Base_object {

    public:

      float radius;
      glm::vec3 center;

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

  struct Capsule {
    glm::vec3 base;
    glm::vec3 tip;
    float radius;
    
    Capsule();
    Capsule(glm::vec3 b, glm::vec3 t, float r);
  };

} // namespace sdf

#endif
