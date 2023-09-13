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
  direction_counter{1},
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

void Player::update_position(sdf::Scene scene) {
  this->previous_position = this->position;

  this->velocity = physics::calculate_drag(this->velocity, this->grounded);

  this->velocity = this->velocity - glm::vec3(0.0f, physics::calculate_gravity(this->direction_counter[DOWN]), 0.0f);

  /*
  if (glm::length(this->velocity) > this->max_speed) {
    this->velocity = this->velocity * (this->max_speed / glm::length(this->velocity));
  }
  */

  if (!this->grounded) {
    this->direction_counter[DOWN]++;
  } else {
    this->direction_counter[DOWN] = 1;
  }

  glm::vec3 frame_velocity = velocity * renderer_state.frame_time;

  glm::vec3 new_position = this->position + frame_velocity;

  sdf::Capsule new_bounds = sdf::Capsule(new_position - (this->up * 1.5f), new_position + (this->up * 0.5f), 0.4f);

  std::vector<physics::collision_info> collision_tests = physics::capsule_scene_collision(new_bounds, scene);

  glm::vec3 new_velocity = physics::collision_response(frame_velocity, collision_tests, this->up, this->grounded);

  this->position += new_velocity;
  this->previous_bounds = this->player_bounds;
  this->update_bounds(1, 2);
}
