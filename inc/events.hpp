#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <chrono>
#include <functional>
#include <ratio>
#include <vector>

#include "./player.hpp"
#include "./scene.hpp"
#include "./renderer_state.hpp"

#define PRESSED(A, B) (glfwGetKey(B, A) == GLFW_PRESS)

#define RELEASED(A, B) (glfwGetKey(B, A) == GLFW_RELEASE)

namespace event {

  namespace game {

    void process_input(Player &p, sdf::Scene &s, GLFWwindow *w);

    glm::vec3 key_w(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d);

    glm::vec3 key_a(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d);

    glm::vec3 key_s(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d);

    glm::vec3 key_d(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d);

    glm::vec3 key_space(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d);

    glm::vec3 key_lshift(Player &p, sdf::Scene &s, GLFWwindow *w, std::array<glm::vec3, 6> d);

    void mouse_callback(GLFWwindow *window, double xpos, double ypos);

  }

  class timed_job {

    public:

      std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
     
      std::chrono::duration<unsigned int, std::milli> job_duration;

      int job_duration_ticks;

      bool run_forever;

      std::function<void(unsigned int, unsigned int)> job;

      timed_job(std::function<void(unsigned int, unsigned int)> job_param, const int N, std::vector<timed_job*> &job_list);

      bool check_expired();

      void run();

  };

  void address_active_jobs(std::vector<timed_job*> &all_active_jobs);

}

#endif
