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
 * (includes the functions:
 *  - collision_info sphere_triangle_collision(glm::vec3 center, float radius, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 N);
 *  - closest sphere in capsule section from
 *    collision_info capsule_mesh_collision_CPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh, const glm::mat4 transformation);
 *    )
 *
 * and Collision response implementation from 
 *
 * https://www.youtube.com/watch?v=YR6Q7dUz2uk which itself references
 *
 * https://www.peroxide.dk/papers/collision/collision.pdf and
 *
 * https://arxiv.org/ftp/arxiv/papers/1211/1211.0059.pdf
 *
 * (includes:
 *   glm::vec3 collision_response(glm::vec3 velocity, const std::vector<collision_info> tests, glm::vec3 up, bool &grounded);
 *   )
 *
 * My implementation is not copied from these sources word for word (as their own implementations are in different languages,
 * and not directly applicable to my own codebase) but are inspired by them,
 * and so they should take credit for the details of the implementation and description of methods involved.
 *
 */


namespace physics {

  glm::vec3 tri_plane_normal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);

  glm::vec3 closest_point_on_line(glm::vec3 p0, glm::vec3 p1, glm::vec3 point_ref);

  glm::vec3 project_on_plane(glm::vec3 u, glm::vec3 n);

  float slope_angle(glm::vec3 v1, glm::vec3 v2);

  collision_info sphere_triangle_collision(glm::vec3 center, float radius, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 N);

  collision_info capsule_mesh_collision_CPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh, const glm::mat4 transformation);

  std::vector<collision_info> capsule_scene_collision(const ::sdf::Capsule capsule, sdf::Scene &scene);

  glm::vec3 collision_response(glm::vec3 velocity, const std::vector<collision_info> tests, glm::vec3 up, bool &grounded);

  glm::vec3 calculate_drag(glm::vec3 v, bool ground, unsigned int counter);

  float calculate_gravity(unsigned int d); 

}

#endif
