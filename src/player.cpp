#include "../inc/player.hpp"

namespace event {
  namespace game {
    extern rotation_state player_rotation_state;
  }
}

Player::Player(glm::vec3 spawn_point) :
  fov(45.0f),
  speed(2.0f), 
  facing(glm::vec3(0.0f, 0.0f, 1.0f)),
  body_direction(glm::vec3(0.0f, 0.0f, 1.0f)),
  yaw(-90.0f),
  pitch(0.0f),
  position(spawn_point), 
  previous_position(spawn_point), 
  up(glm::vec3(0.0f, 1.0f, 0.0f)) 
{}

glm::mat4 Player::get_view() {
  return glm::lookAt(this->position, this->position + this->facing, this->up);
}

glm::mat4 Player::get_proj() {
  return glm::perspective(glm::radians(this->fov), SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 1000.0f);
}

void Player::update_dir() {
  glm::vec3 new_facing;
  new_facing.x = cos(glm::radians(event::game::player_rotation_state.yaw)) * cos(glm::radians(event::game::player_rotation_state.pitch));
  new_facing.y = sin(glm::radians(event::game::player_rotation_state.pitch));
  new_facing.z = sin(glm::radians(event::game::player_rotation_state.yaw)) * cos(glm::radians(event::game::player_rotation_state.pitch));
  this->facing = glm::normalize(new_facing);

  glm::vec3 new_body;
  new_body.x = cos(glm::radians(event::game::player_rotation_state.yaw));
  new_body.y = 0.0f;
  new_body.z = sin(glm::radians(event::game::player_rotation_state.yaw));
  this->body_direction = glm::normalize(new_body);
}

