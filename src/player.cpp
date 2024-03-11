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
  player_bounds(sdf::Capsule(spawn_point - (up * 1.5f), spawn_point + (up * 0.5f), 0.4f)),
  object_velocity_last_frame(glm::vec3(0.0f))
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
}

void Player::update_bounds(unsigned int ct, unsigned int tt) {
  this->player_bounds = sdf::Capsule(this->position - (up * 1.5f), this->position + (up * 0.5f), 0.5f);
}

void Player::update_position(sdf::Scene scene) {

  this->fov = 80.0f + 10 * (glm::length(this->velocity * glm::vec3(1.0f, 0.0f, 1.0f)) / 30.0f);
  
  this->previous_position = this->position;

  this->velocity = physics::calculate_drag(this->velocity,
      this->grounded, std::max(
        std::max(this->direction_counter[FORWARDS], this->direction_counter[BACKWARDS]),
        std::max(this->direction_counter[LEFT], this->direction_counter[RIGHT]))); // second parameter unused currently

  this->velocity = this->velocity - glm::vec3(0.0f, physics::calculate_gravity(this->direction_counter[DOWN]), 0.0f);

  if (!this->grounded) {
    this->direction_counter[DOWN]++;
  } else {
    this->direction_counter[DOWN] = 1;
  }

  glm::vec3 frame_velocity =this->velocity * renderer_state.frame_time;

  glm::vec3 new_position = this->position + frame_velocity;

  sdf::Capsule new_bounds = sdf::Capsule(new_position - (this->up * 1.5f), new_position + (this->up * 0.5f), 0.4f);

  std::vector<physics::collision_info> collision_tests = physics::capsule_scene_collision(new_bounds, scene);

  glm::vec3 new_velocity = physics::collision_response(frame_velocity, collision_tests, this->up, this->grounded);
  
  glm::vec3 object_velocity = glm::vec3(0.0f);
  glm::vec3 extra_acceleration = glm::vec3(0.0f);
  for (const auto &ci:collision_tests) {
    if (ci.hit) {
      extra_acceleration += ci.acceleration;
      object_velocity += ci.object_velocity;
    }
  }
  object_velocity *= renderer_state.frame_time;


  // for some reason either new_velocity or frame_time hold incorrect values until used in a calculation.
  // i have no idea why
  // this section below creates a variable that does a calculation with them
  // the preprocessor directives are just so that i dont get warnings for this variable
  // do not put anything within the pragma clang diagnostic ignored "-Wunused-variable" section other than this calculation
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
  volatile glm::vec3 t1 = renderer_state.frame_time / new_velocity;
  volatile glm::vec3 t2 = this->velocity + extra_acceleration + object_velocity;
#pragma clang diagnostic pop


  if (renderer_state.frame_time > 0.00001) {
    this->velocity = glm::vec3(new_velocity / renderer_state.frame_time); // correctly updates velocity post collison, allowing player to
                                                                          // bounce and off object while maintaining speed
    this->velocity += extra_acceleration;
  }
  
  this->position += this->velocity * renderer_state.frame_time + object_velocity;
  this->previous_bounds = this->player_bounds;
  this->update_bounds(1, 2);
}
