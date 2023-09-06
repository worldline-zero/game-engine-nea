#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "./object.hpp"

namespace physics {

  struct collision_info {
    bool hit;
    float depth;
    glm::vec3 surface_normal;
    glm::vec3 penetration_normal;
  };

  glm::vec3 tri_plane_normal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

  glm::vec3 closest_point_on_line(glm::vec3 p0, glm::vec3 p1, glm::vec3 point_ref);

  glm::vec3 project_on_plane(glm::vec3 u, glm::vec3 n);

  collision_info sphere_triangle_collision(glm::vec3 center, float radius, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

  collision_info capsule_mesh_collision_CPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh, const glm::mat4 transformation);

  collision_info capsule_mesh_collision_GPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh);

}

#endif
