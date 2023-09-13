#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#include "./scene.hpp"
#include "./renderer_state.hpp"

#define PI 3.14159

/*
 * Sphere-Triangle and Capsule-Triangle collision detection implementation from
 *
 * https://wickedengine.net/2020/04/26/capsule-collision-detection/
 *
 * and Collision response implementation from 
 *
 * https://www.youtube.com/watch?v=YR6Q7dUz2uk which itself references
 *
 * https://www.peroxide.dk/papers/collision/collision.pdf and
 *
 * https://arxiv.org/ftp/arxiv/papers/1211/1211.0059.pdf
 *
 * My implementation is not copied from these sources literally, but is inspired by them,
 * and so they should take credit for the details of the implementation and description of methods involved.
 *
 */


std::ostream &operator<<(std::ostream &out, const glm::vec3 &vec);


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

  float slope_angle(glm::vec3 v1, glm::vec3 v2);

  collision_info sphere_triangle_collision(glm::vec3 center, float radius, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 N);

  std::vector<collision_info> capsule_mesh_collision_CPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh, const glm::mat4 transformation);

  collision_info capsule_mesh_collision_GPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh);

  std::vector<collision_info> capsule_scene_collision(const ::sdf::Capsule capsule, const ::sdf::Scene scene);

  glm::vec3 collision_response(glm::vec3 velocity, const std::vector<collision_info> tests, glm::vec3 up, bool &grounded);

  glm::vec3 calculate_drag(glm::vec3 v, bool ground);

  float calculate_gravity(unsigned int d); 

}

#endif
