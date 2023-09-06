#include "../inc/player.hpp"

extern struct renderer_state_container renderer_state;

namespace event {
  namespace game {
    extern rotation_state player_rotation_state;
  }
}

Player::Player(glm::vec3 spawn_point) :
  fov(80.0f),
  facing(glm::vec3(0.0f, 0.0f, 1.0f)),
  velocity(glm::vec3(0.0f)),
  yaw(-90.0f),
  pitch(0.0f),
  max_speed(4.0f),
  position(spawn_point), 
  previous_position(spawn_point), 
  up(glm::vec3(0.0f, 1.0f, 0.0f)),
  player_bounds(sdf::Capsule(spawn_point - (up * 1.5f), spawn_point + (up * 0.5f), 0.4f))
{}

glm::mat4 Player::get_view() {
  return glm::lookAt(this->position, this->position + this->facing, this->up);
}

glm::mat4 Player::get_proj() {
  return glm::perspective(glm::radians(this->fov), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 1000.0f);
}

void Player::update_dir(unsigned int ct, unsigned int tt) {
  glm::vec3 new_facing;
  new_facing.x = cos(glm::radians(event::game::player_rotation_state.yaw)) * cos(glm::radians(event::game::player_rotation_state.pitch));
  new_facing.y = sin(glm::radians(event::game::player_rotation_state.pitch));
  new_facing.z = sin(glm::radians(event::game::player_rotation_state.yaw)) * cos(glm::radians(event::game::player_rotation_state.pitch));
  this->facing = glm::normalize(new_facing);
  /*
  glm::vec3 new_body;
  new_body.x = cos(glm::radians(event::game::player_rotation_state.yaw));
  new_body.y = 0.0f;
  new_body.z = sin(glm::radians(event::game::player_rotation_state.yaw));
  this->direction = glm::normalize(new_body);
  */
}

void Player::update_bounds(unsigned int ct, unsigned int tt) {
  this->player_bounds = sdf::Capsule(this->position - (up * 1.5f), this->position + (up * 0.5f), 0.4f);
}

void Player::update_position(unsigned int ct, unsigned int tt) {
  this->previous_position = this->position;
  this->position += velocity * renderer_state.frame_time;
  this->previous_bounds = this->player_bounds;
  this->update_bounds(1, 2);
}

physics::collision_info Player::test_collision(sdf::Scene &scene) {

  std::vector<sdf::Mesh> meshes;
  std::vector<glm::mat4> transformations;

  for (const auto &[vol_id, vol] : scene.volumes) {
    std::visit([&meshes, &transformations](auto v) mutable {
      for (const auto &[obj_id, obj] : v.children) {
        std::visit([&meshes, &transformations](auto o) mutable {
          meshes.push_back(o.mesh);
          transformations.push_back(o.transformation);
        }, obj);
      }
    }, vol);
  }

  physics::collision_info collision_test_result;

  for (int i = 0; i<meshes.size(); i++) {
    collision_test_result = physics::capsule_mesh_collision_CPU(this->player_bounds, meshes[i], transformations[i]);
    if (collision_test_result.hit == true) {
      return collision_test_result;
    }
  }

  return collision_test_result;

}

void Player::collision_response(sdf::Scene &scene) {

  auto collision_test_result = this->test_collision(scene);

  if (collision_test_result.hit == true) {

    glm::vec3 frame_position_delta = this->position - this->previous_position; // this is the same as the current velocity

    std::cout << "position delta: " << frame_position_delta.x << " " << frame_position_delta.y << " " << frame_position_delta.z << std::endl;

    float position_delta_length = glm::length(frame_position_delta) + (EPSILON * 15.0f);

    std::cout << "position delta length: " << position_delta_length << std::endl;

    float reduced_length = position_delta_length - collision_test_result.depth - (EPSILON * 15.0f);

    std::cout << "reduced length: " << reduced_length << std::endl;

    glm::vec3 reduced_movement_vector;
    if (reduced_length > EPSILON) {
      reduced_movement_vector = reduced_length * glm::normalize(frame_position_delta);
    } else {
      reduced_movement_vector = glm::vec3(0.0f);
    }

    std::cout << "reduced movement vector: " << reduced_movement_vector.x << " " << reduced_movement_vector.y << " " << reduced_movement_vector.z << std::endl;

    glm::vec3 leftover_vector = frame_position_delta - reduced_movement_vector;

    float leftover_magnitude = glm::length(leftover_vector);

    std::cout << "leftover vector: " << leftover_vector.x << " " << leftover_vector.y << " " << leftover_vector.z << std::endl;

    glm::vec3 projected_leftover_vector = leftover_magnitude * glm::normalize(physics::project_on_plane(leftover_vector, -collision_test_result.surface_normal));

    std::cout << "projected vector: " << projected_leftover_vector.x << " " << projected_leftover_vector.y << " " << projected_leftover_vector.z << std::endl;

    glm::vec3 final_position = this->previous_position + reduced_movement_vector + projected_leftover_vector;

    this->position = final_position;
    this->update_bounds(1, 2);

    std::cout << std::endl;

  }

}
