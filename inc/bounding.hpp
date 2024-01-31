#ifndef BOUNDING_H
#define BOUNDING_H

#include <glm/glm.hpp>

#include <map>
#include <variant>
#include <vector>

#include "./object.hpp"

namespace sdf {


  class AABB {

    public:

      unsigned int ID;

      glm::vec3 position;
      glm::vec3 dimensions;
      glm::mat4 transformation;

      std::map<unsigned int, sdf::Object> children;

      AABB(glm::vec3 p, glm::vec3 d);

      void construct_matrix();

      void render(Shader &s, Shader &l, glm::vec3 position) const;

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
