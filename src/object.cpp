#include "../inc/object.hpp"

namespace sdf {

  extern unsigned int objectID;

  unsigned int objectID = 0;

  /*

  float Base_object::get_scale_factor_svd(glm::mat4 t) {
    t = glm::inverse(t);
    float sx = glm::length(glm::vec3(t[0][0], t[0][1], t[0][2]));
    float sy = glm::length(glm::vec3(t[1][0], t[1][1], t[1][2]));
    float sz = glm::length(glm::vec3(t[2][0], t[2][1], t[2][2]));
    return std::min(sx, std::min(sy, sz));
  }



  Sphere::Sphere() : radius(0.5f), center(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)) {
    //glm::mat4 t = glm::scale(glm::mat4(1.0f), glm::vec3(radius));
    //t = glm::translate(t, glm::vec3(0.0f));
    //glm::mat4 t = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 tr = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), glm::vec3(radius));
    glm::mat4 is = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/radius));
    this->transformation = tr * sc;
    this->inverse_scale_trans = tr * is;
    this->type = Sphere_id;
    this->ID = objectID;
    this->mesh = Mesh("./res/sphere.obj");
    objectID++;
  }


  Sphere::Sphere(float r, glm::vec3 c) : radius(r), center(c), velocity(glm::vec3(0.0f)) {
    //glm::mat4 t = glm::scale(glm::mat4(1.0f), glm::vec3(r));
    //t = glm::translate(t, c);
    //glm::mat4 t1 = glm::translate(glm::mat4(1.0f), c);
    //glm::mat4 t2 = glm::translate(glm::mat4(1.0f), c);
    glm::mat4 tr = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), glm::vec3(radius));
    glm::mat4 is = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/radius));
    this->transformation = tr * sc;
    this->inverse_scale_trans = tr * is;
    this->type = Sphere_id;
    this->ID = objectID;
    this->mesh = Mesh("./res/sphere.obj");
    objectID++;
  }

  void Sphere::details() {
    std::cout 
      << "Sphere details:\n"
      << "ID: " << ID << "\n"
      << "radius: " << radius << "\n"
      << "center: " << center.x << " " << center.y << " " << center.z << std::endl;
  }

  float Sphere::dist(glm::vec3 pos) {
    return glm::length(pos - this->center) - this->radius;
  }



  Cuboid::Cuboid() : half_dimensions(glm::vec3(0.5f)), center(glm::vec3(0.0f)), rotation(glm::vec3(1.0f)), angle(0.0f), velocity(glm::vec3(0.0f)) {
    *
    glm::mat4 t1 = glm::translate(glm::mat4(1.0f), center);
    t1 = glm::scale(t1, half_dimensions);
    t1 = glm::rotate(t1, glm::radians(angle), rotation);
    glm::mat4 t2 = glm::scale(glm::mat4(1.0f), 1.0f/half_dimensions);
    t2 = glm::rotate(t2, glm::radians(angle), rotation);
    t2 = glm::translate(t2, center);
    /
    glm::mat4 tr = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 ro = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), half_dimensions);
    glm::mat4 is = glm::scale(glm::mat4(1.0f), 1.0f/half_dimensions);

    this->transformation = tr * ro * sc;
    this->inverse_scale_trans = tr * ro * is;
    this->type = Cuboid_id;
    this->ID = objectID;
    this->mesh = Mesh("./res/cube.obj");
    objectID++;
  }

  Cuboid::Cuboid(glm::vec3 d, glm::vec3 c, glm::vec3 r, float a) : half_dimensions(d), center(c), rotation(r), angle(a), velocity(glm::vec3(0.0f)) {
    glm::mat4 t1 = glm::translate(glm::mat4(1.0f), c);
    t1 = glm::scale(t1, d);
    t1 = glm::rotate(t1, glm::radians(-a), r);
    glm::mat4 t2 = glm::scale(glm::mat4(1.0f), 1.0f/d);
    t2 = glm::rotate(t2, glm::radians(a), r);
    t2 = glm::translate(t2, c);

    glm::mat4 tr = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 ro = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), half_dimensions);
    glm::mat4 is = glm::scale(glm::mat4(1.0f), 1.0f/half_dimensions);

    this->transformation = tr * ro * sc;
    this->inverse_scale_trans = tr * ro * is;
    this->type = Cuboid_id;
    this->ID = objectID;
    this->mesh = Mesh("./res/cube.obj");
    objectID++;
  }

  void Cuboid::details() {
    std::cout 
      << "Cuboid details:\n"
      << "ID: " << ID << "\n"
      << "half dimensions: " << half_dimensions.x << " " << half_dimensions.y << " " << half_dimensions.z << "\n"
      << "center: " << center.x << " " << center.y << " " << center.z << std::endl;
  }

  float Cuboid::dist(glm::vec3 pos) {
    pos = glm::vec3(this->inverse_scale_trans * glm::vec4(-pos, 1.0f));
    glm::vec3 q = glm::abs(pos) - 1.0f;
    return (glm::length(glm::max(q, 0.0f)) + std::min(std::max(q.x, std::max(q.y, q.z)), 0.0f)) * this->get_scale_factor_svd(this->transformation);
  }

  Arbitrary::Arbitrary(Mesh m) : scale(glm::vec3(1.0f)), center(glm::vec3(0.0f)), rotation(glm::vec3(1.0f)), angle(0.0f), velocity(glm::vec3(0.0f)), mesh(m) {

    glm::mat4 tr = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 ro = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 is = glm::scale(glm::mat4(1.0f), 1.0f/scale);

    this->transformation = tr * ro * sc;
    this->inverse_scale_trans = tr * ro * is;
    this->type = Arbitrary_id;
    this->ID = objectID;
    objectID++;

  }

  Arbitrary::Arbitrary(Mesh m, glm::vec3 s, glm::vec3 p, glm::vec3 r, float a) : scale(s), center(p), rotation(r), angle(a), velocity(glm::vec3(0.0f)), mesh(m) {

    glm::mat4 tr = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 ro = glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation);
    glm::mat4 sc = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 is = glm::scale(glm::mat4(1.0f), 1.0f/scale);

    this->transformation = tr * ro * sc;
    this->inverse_scale_trans = tr * ro * is;
    this->type = Arbitrary_id;
    this->ID = objectID;
    objectID++;

  }

  float Arbitrary::dist(glm::vec3 pos) {
    float d = 2147483648.0f;
    for (const Vertex &v:this->mesh.vertices) {
      d = std::min(d, std::abs(glm::length(pos - glm::vec3(this->transformation * glm::vec4(v.position, 1.0f)))));
    }
    return d;
  }

  void Arbitrary::details() {
    std::cout
      << "Arbitrary mesh details:\n"
      << "center: " << this->center << "\n"
      << "ID: " << this->ID << std::endl;
  }

  */
  
  Object::Object(sdf::Mesh m, glm::vec3 p, glm::vec3 s, glm::vec3 r, float a) :
    position(p),
    scale(s),
    rotation(r),
    angle(a),
    mesh(m),
    velocity(glm::vec3(0.0f))
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
