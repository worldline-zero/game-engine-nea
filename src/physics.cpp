#include "../inc/physics.hpp"

extern struct renderer_state_container renderer_state;

namespace physics {

  glm::vec3 tri_plane_normal(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2) {
    return glm::normalize(glm::cross(p1 - p0, p2 - p0));
  }

  glm::vec3 closest_point_on_line(glm::vec3 p0, glm::vec3 p1, glm::vec3 point_ref) {
    glm::vec3 line = p1 - p0;
    float t = glm::dot(point_ref - p0, line) / glm::dot(line, line);
    return p0 + (std::min(std::max(t, 0.0f), 1.0f) * line);
  }

  glm::vec3 project_on_plane(glm::vec3 u, glm::vec3 n) {
    return u - (((glm::dot(u, n))/(glm::dot(n, n))) * n);
  }

  float slope_angle(glm::vec3 v1, glm::vec3 v2) {
    return acos(glm::dot(v1, v2)/(glm::length(v1) * glm::length(v2)));
  }

  collision_info sphere_triangle_collision(glm::vec3 center, float radius, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 N) {
    collision_info result;
    center += glm::vec3(0.0f, glm::max(glm::dot(glm::vec3(0.0f, -1.0f, 0.0f), N), 0.0f), 0.0f);
    float dist_to_plane = glm::dot(center - p0, N);
    if (dist_to_plane > radius || dist_to_plane < -radius) {
      result.hit = false;
      return result;
    } else {
      glm::vec3 center_proj = center - (N * dist_to_plane);

      glm::vec3 c0 = glm::cross(center_proj - p0, p1 - p0);
      glm::vec3 c1 = glm::cross(center_proj - p1, p2 - p1);
      glm::vec3 c2 = glm::cross(center_proj - p2, p0 - p2);

      bool inside = glm::dot(c0, N) <= 0 && glm::dot(c1, N) <= 0 && glm::dot(c2, N) <= 0;

      float radius2 = radius * radius;

      bool intersects = false;

      glm::vec3 point1 = closest_point_on_line(p0, p1, center);
      glm::vec3 v1 = center - point1;
      float distsq1 = glm::dot(v1, v1);
      intersects |= distsq1 < radius2;

      glm::vec3 point2 = closest_point_on_line(p1, p2, center);
      glm::vec3 v2 = center - point2;
      float distsq2 = glm::dot(v2, v2);
      intersects |= distsq2 < radius2;

      glm::vec3 point3 = closest_point_on_line(p2, p0, center);
      glm::vec3 v3 = center - point3;
      float distsq3 = glm::dot(v3, v3);
      intersects |= distsq3 < radius2;

      if (intersects || inside) {

        glm::vec3 intersection_vector;
        glm::vec3 closest_point = center_proj;

        if (inside) {
          intersection_vector = center - center_proj;
        } else {
          glm::vec3 d = center - point1;
          float best_distsq = glm::dot(d, d);
          intersection_vector = d;
          closest_point = point1;

          d = center - point2;
          float distsq = glm::dot(d, d);
          if (distsq < best_distsq) {
            best_distsq = distsq;
            intersection_vector = d;
            closest_point = point2;
          }

          d = center - point3;
          distsq = glm::dot(d, d);
          if(distsq < best_distsq) {
            best_distsq = distsq;
            intersection_vector = d;
            closest_point = point3;
          }
        }
        float len = glm::length(intersection_vector);
        result.hit = true;
        result.depth = radius - len;
        result.surface_normal = N;
        result.penetration_normal = intersection_vector / len;
        return result;

      }

      result.hit = false;
      return result;

    }
  }

  collision_info capsule_mesh_collision_CPU(const ::sdf::Capsule capsule, const ::sdf::Mesh mesh, glm::mat4 transformation) {
    glm::vec3 capsule_normal = glm::normalize(capsule.tip - capsule.base);
    glm::vec3 line_end_offset = capsule_normal * capsule.radius;
    glm::vec3 capsule_A = capsule.base + line_end_offset;
    glm::vec3 capsule_B = capsule.tip - line_end_offset;

    collision_info sphere_test;
    sphere_test.depth = 0.0f;

    //std::cout << "mesh size in vertices: " << mesh.vertices.size() << std::endl;

    for (int i = 0; i<=mesh.vertices.size(); i+=3) {
      //std::cout << i << std::endl;
      glm::vec3 p0 = glm::vec3(transformation * glm::vec4(mesh.vertices[i].position, 1.0f));
      glm::vec3 p1 = glm::vec3(transformation * glm::vec4(mesh.vertices[i+1].position, 1.0f));
      glm::vec3 p2 = glm::vec3(transformation * glm::vec4(mesh.vertices[i+2].position, 1.0f));

      glm::vec3 N = tri_plane_normal(p0, p1, p2);

      float t = glm::dot(N, (p0 - capsule.base) / glm::abs(glm::dot(N, capsule_normal)));
      glm::vec3 line_plane_intersection;
      if (glm::dot(N, capsule_normal) != 0) {
        line_plane_intersection = capsule.base + (t * capsule_normal);
      } else {
        line_plane_intersection = p0;
      }

      //bool parallel = (glm::dot(N, capsule_normal) == 0);
      //std::cout << "parallel? " << parallel << std::endl;

      glm::vec3 c0 = glm::cross(line_plane_intersection - p0, p1 - p0);
      glm::vec3 c1 = glm::cross(line_plane_intersection - p1, p2 - p1);
      glm::vec3 c2 = glm::cross(line_plane_intersection - p2, p0 - p2);

      bool inside = glm::dot(c0, N) <= 0 && glm::dot(c1, N) <= 0 && glm::dot(c2, N) <= 0;

      glm::vec3 reference_point;
    

      if (inside) {
        reference_point = line_plane_intersection;
      } else {
        glm::vec3 point1 = closest_point_on_line(p0, p1, line_plane_intersection);
        glm::vec3 v1 = line_plane_intersection - point1;
        float distsq1 = glm::dot(v1, v1);
        glm::vec3 point2 = closest_point_on_line(p1, p2, line_plane_intersection);
        glm::vec3 v2 = line_plane_intersection - point2;
        float distsq2 = glm::dot(v2, v2);
        glm::vec3 point3 = closest_point_on_line(p2, p0, line_plane_intersection);
        glm::vec3 v3 = line_plane_intersection - point3;
        float distsq3 = glm::dot(v3, v3);
        if (distsq1 <= distsq2 && distsq1 <= distsq3) {
          reference_point = point1;
        } else if (distsq2 <= distsq1 && distsq2 <= distsq3) {
          reference_point = point2;
        } else if (distsq3 <= distsq1 && distsq3 <= distsq2) {
          reference_point = point3;
        }
      }

      glm::vec3 sphere_center = closest_point_on_line(capsule_A, capsule_B, reference_point);

      //std::cout << p0 << '\n' << p1 << '\n' << p2 << '\n' << sphere_center << '\n' << std::endl;


      //std::cout << "current sphere center: " << sphere_center.x << " " << sphere_center.y << " " << sphere_center.z << std::endl;
      //std::cout << "current triangle coords:\n"
      //  << "p0: " << p0.x << " " << p0.y << " " << p0.z << "\n"
      //  << "p1: " << p1.x << " " << p1.y << " " << p1.z << "\n"
      //  << "p2: " << p2.x << " " << p2.y << " " << p2.z << "\n";

      collision_info temp_test = sphere_triangle_collision(sphere_center, capsule.radius, p0, p1, p2, N);
      //collision_info temp_test2 = sphere_triangle_collision(sphere_center, capsule.radius, p0, p1, p2, -N);

      if (p2 == glm::vec3(90, -6, 0) && p1 == glm::vec3(-110, -6, 0) && p0 == glm::vec3(-110, -6, -20)) {
        std::cout << p0 << '\n' << p1 << '\n' << p2 << '\n' << sphere_center << '\n' << temp_test.depth << '\n' << temp_test.hit << std::endl;
        if (temp_test.hit) {
          //exit(0);
        }
      }

      if (temp_test.depth > sphere_test.depth) {
        sphere_test = temp_test;
      }
        
      //std::cout << std::endl;

    } // for loop end

    return sphere_test;

  }

  std::vector<collision_info> capsule_scene_collision(const ::sdf::Capsule capsule, sdf::Scene &scene) {

    std::vector<collision_info> collisions;

    for (auto &[vol_id, vol] : scene.volumes) {
      for (auto &[obj_id, obj] : vol.children) {
        auto collision_test_result = physics::capsule_mesh_collision_CPU(capsule, obj.mesh, obj.transformation);
        if (obj.solid) {
          collision_test_result.object_velocity = obj.velocity;
          collisions.push_back(collision_test_result);
        }
        if (collision_test_result.hit) {
          obj.collision_behaviour(collision_test_result);
        }
      }
    }

    return collisions;

  }

  glm::vec3 collision_response(glm::vec3 velocity, const std::vector<collision_info> tests, glm::vec3 up, bool &ground) {
    glm::vec3 normal = glm::vec3(0.0f);
    bool hit = false;
    float depth = 0.0f;
    glm::vec3 collison_velocity = glm::vec3(0.0f);
    for (const auto &i : tests) {
      hit |= i.hit;
      if (i.hit && (i.penetration_normal == i.penetration_normal)) {
        depth = std::max(std::abs(i.depth), depth);
        normal += i.penetration_normal;
        collison_velocity += project_on_plane(i.object_velocity, i.penetration_normal);
      }
    }

    //velocity += collison_velocity;

    if (hit == true) {
      //std::cout << normal << std::endl;
      if (glm::length(normal) <= EPSILON) {
        return glm::vec3(0.0f);
      } else {
        normal = glm::normalize(normal);
      }
      if (glm::length(velocity) < 0.001f) {
        return glm::vec3(0.0f);
      }
      glm::vec3 reduced_velocity = velocity * ((glm::length(velocity) - depth - EPSILON) / glm::length(velocity));
      if (glm::length(reduced_velocity) < 0.015f) {
        reduced_velocity = glm::vec3(0.0f);
      }
      glm::vec3 leftover_velocity = velocity - reduced_velocity;
      float ground_angle = slope_angle(up, normal);
      if (glm::dot(normal, glm::normalize(velocity)) > 0.0f) {
        return velocity;
      } else {
        leftover_velocity = project_on_plane(leftover_velocity, normal);
      }

      if (ground_angle < PI/4.0f) {
        ground = true;
        return project_on_plane(velocity, normal);
      } else {
        ground = false;
      }

      return reduced_velocity + leftover_velocity;// + (std::abs(glm::dot(velocity, normal)) * (glm::normalize(reduced_velocity + leftover_velocity) + normal) * glm::length(reduced_velocity + leftover_velocity));

    } else {

      ground = false;

      return velocity;

    }
  }

  glm::vec3 calculate_drag(glm::vec3 v, bool ground, unsigned int counter) {
    if (ground) {
      return v * glm::vec3(0.95f - (2 * renderer_state.frame_time));
    } else {
      return v * glm::vec3(0.99f, 0.94f, 0.99f);
    }
  }

  float calculate_gravity(unsigned int d) {
    return d * renderer_state.frame_time;
  }

}
