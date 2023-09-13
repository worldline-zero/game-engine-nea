#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <array>

#include "../inc/start_opengl.hpp"

#include "../inc/scene.hpp"
#include "../inc/physics.hpp"
#include "../inc/renderer_state.hpp"

namespace event {
  namespace game {
    struct rotation_state {
      float yaw = -90.0f;
      float pitch = 0.0f;
      float lastX = SCREEN_WIDTH/2.0f;
      float lastY = SCREEN_HEIGHT/2.0f;
    };
  }
}

enum directions {
  FORWARDS,
  BACKWARDS,
  UP,
  DOWN,
  RIGHT,
  LEFT
};

class Player {

  public:

    bool grounded; // whether or not the player is grounded

    float fov; // field of view in degrees
    
    glm::vec3 facing; // normalized vector. seperate to speed because this needs to be used to calculate the lookat matrix
    glm::vec3 velocity;
    float yaw, pitch, lastY, lastX;

    float max_speed;

    std::array<unsigned int, 6> direction_counter;


    glm::vec3 position; // player's current position
    glm::vec3 previous_position; // players position last frame
    glm::vec3 up; // the up vector. probably could be const/literal but incase i decide to do wacky shit with gravity in the future ill keep it

    sdf::Capsule player_bounds;
    sdf::Capsule previous_bounds;

    Player(glm::vec3 spawn_point);

    glm::mat4 get_view();

    glm::mat4 get_proj();

    void update_dir(unsigned int ct, unsigned int tt);

    void update_bounds(unsigned int ct, unsigned int tt);

    void update_position(sdf::Scene scene);

};

#endif
