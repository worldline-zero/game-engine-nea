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

  sdf::Scene test_scene;

  sdf::bounding_sphere test_volume(glm::vec3(2.0f, 0.0f, 0.0f), 4.0f);

  sdf::AABB test_aabb(glm::vec3(0.0f), glm::vec3(25.0f, 10.0f, 25.0f));

  sdf::Cuboid test_cube1(glm::vec3(1.0f), glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 40.0f);
  sdf::Cuboid test_cube2;

  sdf::Sphere test_sphere(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  sdf::Sphere test_sphere2(0.5f, glm::vec3(-1.5f, 1.0f, 0.0f));

  sdf::Cuboid test_floor(glm::vec3(10.0f, 0.10f, 1000.0f), glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 20.0f);

  test_aabb.add_object(test_floor);

  test_volume.add_object(test_cube1);
  test_volume.add_object(test_cube2);
  test_volume.add_object(test_sphere);
  test_volume.add_object(test_sphere2);

//  sdf::Cuboid start_zone_mesh(glm::vec3(1.0f, 5.0f, 5.0f), glm::vec3(-10.0f), glm::vec3(1.0f), 0.0f);

  /*

  level::start_zone start(glm::vec3(1.0f, 5.0f, 5.0f), glm::vec3(-10.0f), glm::vec3(1.0f), 0.0f);
  level::end_zone end(glm::vec3(1.0f, 5.0f, 5.0f), glm::vec3(10.0f), glm::vec3(1.0f), 0.0f);

  start.collision_behaviour = [&start, &test_jobs](physics::collision_info c) mutable {
    if (start.collided == false) {
      start.opt_timed_job = event::timed_job(
        [](unsigned int ct, unsigned int tt) {
          std::cout << ct << std::endl;
        }, -1);
      start.opt_timed_job->add_to(test_jobs);
      start.collided = true;
    }
  };

  start.solid = false;

  end.collision_behaviour = [&window](physics::collision_info c) mutable {
    glfwSetWindowShouldClose(window, true);
  };

  end.solid = false;

  test_aabb.add_object(start);
  test_aabb.add_object(end);

  */

  for (int i = -1; i<12; i++) {
    test_aabb.add_object(sdf::Cuboid(glm::vec3(0.7f, 0.1f, 0.7f), glm::vec3(2*i, i, 2*i), glm::vec3(1.0f), 0.0f));
  }

  test_scene.add_volume(test_volume);
  test_scene.add_volume(test_aabb);

  Player player(glm::vec3(5.0f));

  std::function<void(unsigned int, unsigned int)> test_lambda = [](unsigned int ct, unsigned int tt){
    std::cout << (float)ct / (float)tt << std::endl;
  };

  event::timed_job test_event(test_lambda, 1000);
  //event::timed_job frame_info_job(render::print_frame_info, -1);
  event::timed_job render_state_update_job(render::update_render_state, -1);
  //test_event.add_to(test_jobs);
  render_state_update_job.add_to(test_level.jobs);

  auto &vol = test_level.scene[0];

  std::visit([](auto &v) {
    v.add_object(sdf::Cuboid(glm::vec3(100.0f, 1.0f, 10.0f), glm::vec3(1.0f), glm::vec3(1.0f), 0.0f));
  }, vol);

  std::visit([](auto &v) {
    std::visit([](auto &c) {
      std::cout << c.ID << std::endl;
      c.velocity = glm::vec3(3.0f, 0.0f, 0.0f);
    }, v[1]);
  }, vol);

  std::visit([](auto &v) {
    std::visit([](auto &c) {
      std::cout << c.velocity << std::endl;
    }, v[1]);
  }, vol);


  test_level.running = true;

  while (test_level.running) {

    player.update_dir(1, 2);

    test_level.scene.update();

    event::game::process_input(player, test_level.scene, window);

    player.update_position(test_level.scene);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screen.use();

    screen.set_matrix<glm::mat4>("view", player.get_view());
    screen.set_matrix<glm::mat4>("projection", player.get_proj());

    test_level.scene.render(screen);

    std::cout << test_level.jobs.size() << std::endl;

    //event::address_active_jobs(test_jobs);
    event::address_active_jobs(test_level.jobs);

    //std::cout << player.position << std::endl;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  std::cout << "program terminated successfully\n";

  glfwTerminate();

  return 0;

}
