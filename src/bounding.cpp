#include "../inc/bounding.hpp"

namespace sdf {

  extern unsigned int objectID;

  /*

  void bounding_volume::print_children() {
    std::cout << "\n";
    for (const auto &[id_, obj_]:children) {
      std::visit([](auto o){o.details(); std::cout << "\n";}, obj_);
    }
  }

  void bounding_volume::render(Shader &s) {
    for (const auto &[obj_id, obj] : this->children) {
      std::visit([&s](auto o) { 
        s.set_matrix<glm::mat4>("model", o.transformation);
        o.mesh.draw();
      }, obj);
    }
  }



  AABB::AABB() : center(glm::vec3(0.0f)), half_dimensions(glm::vec3(0.5f)) {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), center);
    this->transformation = glm::scale(t, half_dimensions);
    this->type = AABB_id;
    this->ID = objectID;
    objectID++;
  }

  AABB::AABB(glm::vec3 c, glm::vec3 half_d) : center(c), half_dimensions(half_d) {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), center);
    this->transformation = glm::scale(t, half_dimensions);;
    this->type = AABB_id;
    this->ID = objectID;
    objectID++;
  }

  void AABB::details() {
    std::cout 
      << "AABB details:\n"
      << "ID: " << ID << "\n"
      << "center: " << center.x << " " << center.y << " " << center.z << "\n"
      << "half dimensions: " << half_dimensions.x << " " << half_dimensions.y << " " << half_dimensions.z << std::endl;
  }

  float AABB::dist(glm::vec3 pos) {
    glm::vec3 q = glm::abs(pos - this->center) - this->half_dimensions;
    return glm::length(glm::max(q, 0.0f)) + std::min(std::max(q.x, std::max(q.y, q.z)), 0.0f);
  }



  bounding_sphere::bounding_sphere() : center(glm::vec3(0.0f)), radius(0.5f) {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), center);
    this->transformation = glm::scale(t, glm::vec3(radius));
    this->type = bounding_sphere_id;
    this->ID = objectID;
    objectID++;
  }

  bounding_sphere::bounding_sphere(glm::vec3 c, float r) : center(c), radius(r) {
    glm::mat4 t = glm::translate(glm::mat4(1.0f), center);
    this->transformation = glm::scale(t, glm::vec3(r));
    this->type = bounding_sphere_id;
    this->ID = objectID;
    objectID++;
  }

  void bounding_sphere::details() {
    std::cout
      << "Bounding Sphere details:\n"
      << "ID: " << ID << "\n"
      << "center: " << center.x << " " << center.y << " " << center.z << "\n"
      << "radius: " << radius << std::endl;
  }

  float bounding_sphere::dist(glm::vec3 pos) {
    return glm::length(pos - this->center) - this->radius;
  }
  */

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

  void AABB::render(Shader &s) const {
    for (const auto &[obj_id, obj]:this->children) {
      s.set_matrix<glm::mat4>("model", obj.transformation);
      obj.mesh.draw();
    }
  }

  void AABB::add_object(sdf::Object object) {
    this->children.insert(std::make_pair(object.ID, object));
  }

}
