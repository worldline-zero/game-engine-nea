#ifndef BOUNDING_H
#define BOUNDING_H

#include <glm/glm.hpp>

#include <map>
#include <variant>
#include <vector>

#include "./object.hpp"

namespace sdf {

  /*

  class bounding_volume {

    public:
      
      using child_type = std::variant<Sphere, Cuboid, Arbitrary>;

      using child_container = std::map<unsigned int, child_type>;


      child_container children;

      glm::mat4 transformation;
      unsigned int ID;
      object_type_id type;


      template <class T>
      void add_object(const T object) {
        child_type new_child = object;
        children.insert(std::make_pair(object.ID, new_child));
      }

      void print_children();

      void render(Shader &s);

      child_type &operator[](const unsigned int id) {
        return children.at(id);
      }

  };

  class AABB : public bounding_volume {

    public:

      glm::vec3 center;
      glm::vec3 half_dimensions;

      AABB();

      AABB(glm::vec3 c, glm::vec3 half_d);

      void details();

      float dist(glm::vec3 pos);

  };

  class bounding_sphere : public bounding_volume {

    public:

      glm::vec3 center;
      float radius;

      bounding_sphere();

      bounding_sphere(glm::vec3 c, float r);

      void details();

      float dist(glm::vec3 pos);

  };

  */

  class AABB {

    public:

      unsigned int ID;

      glm::vec3 position;
      glm::vec3 dimensions;
      glm::mat4 transformation;

      std::map<unsigned int, sdf::Object> children;

      AABB(glm::vec3 p, glm::vec3 d);

      void construct_matrix();

      void render(Shader &s) const;

      void add_object(sdf::Object);

      sdf::Object operator[](unsigned int id) {
        return this->children.at(id);
      }

      sdf::AABB operator=(const sdf::AABB &other) {
        this->position = other.position;
        this->dimensions = other.dimensions;
        this->transformation = other.transformation;
        this->children = other.children;
        return *this;
      }

  };

} // namespace sdf

#endif
