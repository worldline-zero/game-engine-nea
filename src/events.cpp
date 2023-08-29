#include "../inc/events.hpp"

namespace event {

  namespace game {

    extern struct rotation_state player_rotation_state;

    rotation_state player_rotation_state;

    void process_input(Player &p, sdf::Scene &s, GLFWwindow *w) {

      if (PRESSED(GLFW_KEY_W, w)) {
        key_w(p, s, w);
      }
      
      if (PRESSED(GLFW_KEY_A, w)) {
        key_a(p, s, w);
      }

      if (PRESSED(GLFW_KEY_S, w)) {
        key_s(p, s, w);
      }

      if (PRESSED(GLFW_KEY_D, w)) {
        key_d(p, s, w);
      }

      if (PRESSED(GLFW_KEY_SPACE, w)) {
        key_space(p, s, w);
      }

      if (PRESSED(GLFW_KEY_LEFT_SHIFT, w)) {
        key_lshift(p, s, w);
      }

      if (PRESSED(GLFW_KEY_ESCAPE, w)) {
        key_esc(p, s, w);
      }

    }

    void key_esc(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glfwSetWindowShouldClose(w, true);
    }

    void key_w(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 old = p.position;
      p.position += p.body_direction * p.speed;
      p.position.y = old.y;
    }

    void key_a(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 old = p.position;
      p.position -= glm::normalize(glm::cross(p.body_direction, p.up)) * p.speed;
      p.position.y = old.y;
    }

    void key_s(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 old = p.position;
      p.position -= p.body_direction * p.speed;
      p.position.y = old.y;
    }

    void key_d(Player &p, sdf::Scene &s, GLFWwindow *w) {
      glm::vec3 old = p.position;
      p.position += glm::normalize(glm::cross(p.body_direction, p.up)) * p.speed;
      p.position.y = old.y;
    }

    void key_space(Player &p, sdf::Scene &s, GLFWwindow *w) {
      p.position.y += p.speed;
    }

    void key_lshift(Player &p, sdf::Scene &s, GLFWwindow *w) {
      p.position.y -= p.speed;
    }

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
