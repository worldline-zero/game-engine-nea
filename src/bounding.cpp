#include "../inc/bounding.hpp"

namespace sdf {

  extern unsigned int objectID;


  // AABB is an abstract object and has no effect on how the program functions. it is merely a container for grouping purpsoes
  AABB::AABB(glm::vec3 p, glm::vec3 d) :
    position(p),
    dimensions(d)
  {
    construct_matrix();
    this->ID = objectID;
    objectID++;
  }

  void AABB::construct_matrix() {
    glm::mat4 tr = glm::translate(this->transformation, this->position);
    glm::mat4 sc = glm::scale(this->transformation, this->dimensions);
    this->transformation = tr * sc;
  }

  void AABB::render(Shader &s, Shader &l, glm::vec3 position) const {
    std::map<float, sdf::Object> sorted_transparent_objects; // must be rendered in order to make transparaency work
    for (const auto &[obj_id, obj]:this->children) {
      if (obj.color.w < 255) { // if it isnt opaque
        float distance_to_object = glm::length(position - obj.position);
        sorted_transparent_objects.insert(std::make_pair(distance_to_object, obj)); // automatically sorted by distance
      } else {
        if (obj.light_intensity > 0.0f) { // if its a light
          l.use();
          l.set_matrix<glm::mat4>("model", obj.transformation);
          l.set_vector<glm::vec4>("input_color", obj.color / glm::vec4(255.0f));
        } else {
          s.use();
          s.set_matrix<glm::mat4>("model", obj.transformation);
          s.set_vector<glm::vec4>("input_color", obj.color / glm::vec4(255.0f));
        }
        obj.mesh.draw();
      }
    }

    for (const auto &[t_id, tp_obj]:sorted_transparent_objects) { // draw transparent objects in order

      if (tp_obj.light_intensity > 0.0f) {
        l.use();
        l.set_matrix<glm::mat4>("model", tp_obj.transformation);
        l.set_vector<glm::vec4>("input_color", tp_obj.color / glm::vec4(255.0f));
      } else {
        s.use();
        s.set_matrix<glm::mat4>("model", tp_obj.transformation);
        s.set_vector<glm::vec4>("input_color", tp_obj.color / glm::vec4(255.0f));
      }
      tp_obj.mesh.draw(); 

    }
  }

  void AABB::add_object(sdf::Object object) {
    this->children.insert(std::make_pair(object.ID, object));
  }

}
