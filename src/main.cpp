#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../lib/include/glad/gl.h"
#include "../inc/start_opengl.hpp"
#include "../inc/texture.hpp"
#include "../inc/shader.hpp"
#include "../inc/scene.hpp"
#include "../inc/events.hpp"
#include "../inc/player.hpp"
#include "../inc/renderer_state.hpp"

extern struct renderer_state_container renderer_state;

int main() {

  std::cout << std::boolalpha;

  GLFWwindow* window = loader::load_gl();
  glfwSetCursorPosCallback(window, event::game::mouse_callback);
  glfwSwapInterval(0);

  if (window == NULL) {
    std::cerr << "error: window not initialised" << std::endl;
    exit(1);
  }

  Shader screen("./shader/vertex.glsl", "./shader/frag.glsl");

  std::string message = "hello world";

  sdf::Scene test_scene;

  sdf::bounding_sphere test_volume(glm::vec3(2.0f, 0.0f, 0.0f), 4.0f);

  sdf::Cuboid test_cube1(glm::vec3(1.0f), glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 40.0f);
  sdf::Cuboid test_cube2;

  sdf::Sphere test_sphere(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  sdf::Sphere test_sphere2(0.5f, glm::vec3(-1.5f, 1.0f, 0.0f));

  test_volume.add_object(test_cube1);
  test_volume.add_object(test_cube2);
  test_volume.add_object(test_sphere);
  test_volume.add_object(test_sphere2);

  test_scene.add_volume(test_volume);

  Player player(glm::vec3(5.0f));

  std::vector<event::timed_job*> test_jobs;

  std::function<void(unsigned int, unsigned int)> test_lambda = [](unsigned int ct, unsigned int tt){
    std::cout << (float)ct / (float)tt << std::endl;
  };

  //event::timed_job test_event(test_lambda, 1000, test_jobs);
  //event::timed_job frame_info_job(render::print_frame_info, -1, test_jobs);
  event::timed_job render_state_update_job(render::update_render_state, -1, test_jobs);

  while (!glfwWindowShouldClose(window)) {

    player.update_dir(1, 2);

    event::game::process_input(player, test_scene, window);

    player.update_position(test_scene);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screen.use();

    screen.set_matrix<glm::mat4>("view", player.get_view());
    screen.set_matrix<glm::mat4>("projection", player.get_proj());

    test_scene.render(screen);

    event::address_active_jobs(test_jobs);

    std::cout << player.position.x << " " << player.position.y << " " << player.position.z << std::endl;

    //std::cout << test_sphere2.dist(player.position) << std::endl;
  
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;

}
