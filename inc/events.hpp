#ifndef EVENTS_H
#define EVENTS_H

#include <iostream>
#include <glm/glm.hpp>
#include <array>
#include <chrono>
#include <functional>
#include <ratio>
#include <vector>

#include "../inc/player.hpp"
#include "../inc/scene.hpp"
#include "../inc/renderer_state.hpp"
#include "../inc/gui.hpp"

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

    void key_esc(Player &p, sdf::Scene &s, GLFWwindow *w);

    void mouse_callback(GLFWwindow *window, double xpos, double ypos);

  }

  namespace menu {

    void process_input(GLFWwindow *w, gui::GUI *g);

  }

}

#endif
