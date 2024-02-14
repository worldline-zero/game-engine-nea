#include "../inc/object.hpp"

namespace sdf {

  extern unsigned int objectID; // visible to scene.cpp and bounding.cpp

  unsigned int objectID = 0;

  
  Object::Object(sdf::Mesh m, glm::vec3 p, glm::vec3 s, glm::vec3 r, float a) :
    position(p),
    scale(s),
    rotation(r),
    angle(a),
    mesh(m),
    light_intensity(0.0f),
    velocity(glm::vec3(0.0f)),
    acceleration(glm::vec3(0.0f)),
    rotational_velocity(1.0f)
  {
    construct_matrix();
    this->ID = objectID;
    objectID++;
  }

  void Object::construct_matrix() {
    glm::mat4 tr = glm::translate(glm::mat4(1.0f), this->position);
    glm::mat4 ro = glm::rotate(glm::mat4(1.0f), glm::radians(this->angle), this->rotation);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), this->scale);
    this->transformation = tr * ro * sc;
  }
    

  Capsule::Capsule() : base(glm::vec3(0.0f)), tip(glm::vec3(2.0f)), radius(0.5f) {}

  Capsule::Capsule(glm::vec3 b, glm::vec3 t, float r) : base(b), tip(t), radius(r) {}

}
