#include "../inc/events.hpp"

extern struct renderer_state_container renderer_state;

namespace event {

  namespace game {

    extern struct rotation_state player_rotation_state;

    rotation_state player_rotation_state;

    void process_input(Player &p, sdf::Scene &s, GLFWwindow *w) {

      p.previous_position = p.position;
      p.previous_bounds = p.player_bounds;

      // calculate normalized direction vectors from players perspective
      // forwards, backwards, up, down, right, left
      glm::vec3 forwards = glm::normalize(glm::vec3(p.facing.x, 0.0f, p.facing.z));
      glm::vec3 backwards = -forwards;

      glm::vec3 up = p.up;
      glm::vec3 down = -up;

      glm::vec3 right = glm::normalize(glm::cross(forwards, up));
      glm::vec3 left = -right;

      std::array<glm::vec3, 6> directions = { forwards, backwards, up, down, right, left };

      p.velocity += key_w(p, s, w, directions);
      p.velocity += key_a(p, s, w, directions);
      p.velocity += key_s(p, s, w, directions);
      p.velocity += key_d(p, s, w, directions);
      p.velocity += key_space(p, s, w, directions);

      std::cout << p.velocity << std::endl;
     
      /*
      if (glm::length(p.velocity) > p.max_speed) {
        p.velocity = (p.velocity * (p.max_speed / glm::length(p.velocity)));
      }
      */

      //std::cout << glm::length(p.velocity) << std::endl;

    }

    glm::vec3 key_w(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d) {
      glm::vec3 new_velocity = glm::vec3(0.0f);
      if (PRESSED(GLFW_KEY_W ,w)) {
        new_velocity = log2f(p.direction_counter[FORWARDS]) * d[FORWARDS] * renderer_state.frame_time;
        if (p.grounded) {
          new_velocity *= 20.0f;
        }
        p.direction_counter[FORWARDS]++;
      } else if (RELEASED(GLFW_KEY_W, w)) {
        p.direction_counter[FORWARDS] = 1;
      }
      return new_velocity;
    }

    glm::vec3 key_a(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d) {
      glm::vec3 new_velocity = glm::vec3(0.0f);
      if (PRESSED(GLFW_KEY_A, w)) {
        new_velocity = log2f(p.direction_counter[LEFT]) * d[LEFT] * renderer_state.frame_time;
        if (p.grounded) {
          new_velocity *= 10.0f;
        }
        p.direction_counter[LEFT]++;
      } else if (RELEASED(GLFW_KEY_A, w)) {
        p.direction_counter[LEFT] = 1;
      }
      return new_velocity;
    }

    glm::vec3 key_s(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d) {
      glm::vec3 new_velocity = glm::vec3(0.0f);
      if (PRESSED(GLFW_KEY_S ,w)) {
        new_velocity = log2f(p.direction_counter[BACKWARDS]) * d[BACKWARDS] * renderer_state.frame_time;
        if (p.grounded) {
          new_velocity *= 7.0f;
        }
        p.direction_counter[BACKWARDS]++;
      } else if (RELEASED(GLFW_KEY_S, w)) {
        p.direction_counter[BACKWARDS] = 1;
      }
      return new_velocity;
    }

    glm::vec3 key_d(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d) {
      glm::vec3 new_velocity = glm::vec3(0.0f);
      if (PRESSED(GLFW_KEY_D, w)) {
        new_velocity = log2f(p.direction_counter[RIGHT]) * d[RIGHT] * renderer_state.frame_time;
        if (p.grounded) {
          new_velocity *= 10.0f;
        }
        p.direction_counter[RIGHT]++;
      } else if (RELEASED(GLFW_KEY_D, w)) {
        p.direction_counter[RIGHT] = 1;
      }
      return new_velocity;
    }

    glm::vec3 key_space(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d) {
      glm::vec3 new_velocity = glm::vec3(0.0f);
      if (PRESSED(GLFW_KEY_SPACE, w) && p.direction_counter[UP] == 1) {
        new_velocity = glm::vec3(0.0f, 20.0f, 0.0f);
        p.direction_counter[UP]++;
        p.direction_counter[DOWN] = 1;
      } else if (RELEASED(GLFW_KEY_SPACE, w)) {
        p.direction_counter[UP] = 1;
      }
      return new_velocity;
    }

/*

    void key_esc(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glfwSetWindowShouldClose(w, true);
    }

    void key_w(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 direction = glm::normalize(glm::vec3(p.facing.x, 0.0f, p.facing.y));
      if (glm::length(p.velocity) < p.max_speed) {
        p.velocity += p.direction;
      }
    }

    void key_a(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 direction = glm::normalize(glm::vec3(p.facing.x, 0.0f, p.facing.y));
      if (glm::length(p.velocity) < p.max_speed) {
        p.velocity -= glm::normalize(glm::cross(glm::normalize(p.direction), p.up)) * glm::length(p.velocity);
      }
    }

    void key_s(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 direction = glm::normalize(glm::vec3(p.facing.x, 0.0f, p.facing.y));
      glm::vec3 old = p.position;
      p.position -= p.velocity;
      p.position.y = old.y;
    }

    void key_d(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 direction = glm::normalize(glm::vec3(p.facing.x, 0.0f, p.facing.y));
      glm::vec3 old = p.position;
      p.position += glm::normalize(glm::cross(glm::normalize(p.velocity), p.up)) * glm::length(p.velocity);
      p.position.y = old.y;
    }

    void key_space(Player &p, sdf::Scene &s, GLFWwindow *w) {
      p.position.y += glm::length(p.velocity);
    }

    void key_lshift(Player &p, sdf::Scene &s, GLFWwindow *w) {
      p.position.y -= glm::length(p.velocity);
    }

    */

    void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
      float xoffset = xpos - player_rotation_state.lastX;
      float yoffset = player_rotation_state.lastY - ypos;
      player_rotation_state.lastX = xpos;
      player_rotation_state.lastY = ypos;

      float sensitivity = 0.1f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      player_rotation_state.yaw += xoffset;
      player_rotation_state.pitch += yoffset;

      if (player_rotation_state.pitch > 89.0f) {
        player_rotation_state.pitch = 89.0f;
      }
      if (player_rotation_state.pitch < -89.0f) {
        player_rotation_state.pitch = -89.0f;
      }

    }

  } // namespace game


  timed_job::timed_job(std::function<void(unsigned int, unsigned int)> job_param, const int N, std::vector<timed_job*> &job_list) :
    start_time(std::chrono::high_resolution_clock::now()),
    //job_duration(std::chrono::duration<unsigned int, std::milli>(N)),
    job_duration_ticks(N),
    job(job_param) {

    job_list.push_back(this);
    if (N < 0) {
      this->run_forever = true;
      this->job_duration = std::chrono::duration<unsigned int, std::milli>(0);
    } else {
      this->run_forever = false;
      this->job_duration = std::chrono::duration<unsigned int, std::milli>((unsigned int)N);
    }
  }

  bool timed_job::check_expired() {
    if (!run_forever) {
      auto current_time = std::chrono::high_resolution_clock::now();
      unsigned int total_time_elapsed = std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(current_time - this->start_time).count();
      //std::cout << total_time_elapsed << std::endl;
      if (total_time_elapsed >= this->job_duration.count()) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  void timed_job::run() {
    this->job(
        std::chrono::duration_cast<std::chrono::duration<unsigned int, std::milli>>(std::chrono::high_resolution_clock::now() - this->start_time).count(),
        this->job_duration_ticks
        );
  }

  
  void address_active_jobs(std::vector<timed_job*> &all_active_jobs) {
    for (size_t i = 0; i<all_active_jobs.size(); i++) {
      if (all_active_jobs[i]->check_expired() == false) {
        all_active_jobs[i]->run();
      } else {
        all_active_jobs[i]->run();
        all_active_jobs.erase(all_active_jobs.begin() + i);
      }
    }
  }

}
