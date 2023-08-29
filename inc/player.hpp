#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "../inc/start_opengl.hpp"

#include "../inc/object.hpp"
#include "../inc/physics.hpp"

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


class Player {

  public:

    bool grounded; // whether or not the player is grounded

    float fov; // field of view in degrees
    
    float speed; // in units per second (will be adjusted for frame times)
    
    glm::vec3 facing; // normalized vector. seperate to speed because this needs to be used to calculate the lookat matrix
    glm::vec3 body_direction;
    float yaw, pitch, lastY, lastX;


    glm::vec3 position; // player's current position
    glm::vec3 previous_position; // players position last frame
    glm::vec3 up; // the up vector. probably could be const/literal but incase i decide to do wacky shit with gravity in the future ill keep it

    Player(glm::vec3 spawn_point);

    glm::mat4 get_view();

    glm::mat4 get_proj();

    void update_dir();

};

#endif
