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
#include "../inc/level.hpp"

extern struct renderer_state_container renderer_state;

int main() {

  std::cout << std::boolalpha;

  GLFWwindow* window = loader::load_gl();
  glfwSetCursorPosCallback(window, event::game::mouse_callback);
  //glfwSwapInterval(0);

  if (window == NULL) {
    std::cerr << "error: window not initialised" << std::endl;
    exit(1);
  }

  Shader screen("./shader/vertex.glsl", "./shader/frag.glsl");

  std::vector<event::timed_job*> test_jobs;

  level::Level test_level("./level/test.level");

  Player player(glm::vec3(0.0f));

  event::timed_job render_state_update_job(render::update_render_state, -1);
  render_state_update_job.add_to(test_level.jobs, std::string("update_job"));

  //auto &vol = test_level.scene[0];

  //vol.add_object(sdf::Object(sdf::Mesh("/home/charlotte/usr/dev/game-engine-nea/res/cube.obj"), glm::vec3(-10.0f), glm::vec3(100.0f, 4.0f, 10.0f), glm::vec3(1.0f), 0.0f));

 // std::cout << vol[1].ID << std::endl;
 // vol[1].velocity = glm::vec3(3.0f, 0.0f, 0.0f);

  //std::cout << vol[1].velocity << std::endl;


  test_level.running = true;

  while (test_level.running) {

    player.update_dir(1, 2);

    //std::cout << v[2].velocity << std::endl;

    event::game::process_input(player, test_level.scene, window);

    player.update_position(test_level.scene);

    test_level.scene.update();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screen.use();

    screen.set_matrix<glm::mat4>("view", player.get_view());
    screen.set_matrix<glm::mat4>("projection", player.get_proj());


    test_level.scene.render(screen);

    //std::cout << test_level.jobs.size() << std::endl;

    //event::address_active_jobs(test_jobs);
    event::address_active_jobs(test_level.jobs);

    std::cout << player.velocity << std::endl;
    std::cout << player.position << std::endl;

    //char c;
    //std::cin >> c;
    
    std::cout << std::endl;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  std::cout << "program terminated successfully\n";

  glfwTerminate();

  return 0;

}
