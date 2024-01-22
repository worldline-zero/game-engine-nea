#include "../inc/level.hpp"

extern struct renderer_state_container renderer_state;

namespace level {

  Level::Level(const std::string level_path) : running(false) {
    std::ifstream file(level_path);
    std::string src((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    boost::char_separator<char> sep("\n ");
    Tokenizer tokens(src, sep);

    std::vector<std::string> tokens_vector;

    for (Tokenizer::iterator i = tokens.begin(); i!=tokens.end(); i++) {
      tokens_vector.push_back(*i);
    }

    for (std::vector<std::string>::iterator i = tokens_vector.begin(); i!=tokens_vector.end(); i++) {
      if (*i == "include") {
        std::string include_path = *(++i);
        include_file(tokens_vector, i, include_path);
      }
    }

    std::vector<std::string>::iterator i = tokens_vector.begin();

    this->spawn = parse_vec3(i);

    flags_parser fp;

    std::optional<sdf::AABB> current_bounds = std::nullopt;
    for (; i!=tokens_vector.end(); i++) {

      std::cout << *i << std::endl;

      if (*i == "AABB") {
        if (current_bounds == std::nullopt) {
          current_bounds = parse_AABB(i);
        } else {
          this->scene.add_volume(current_bounds.value());
          current_bounds = parse_AABB(i);
        }
      }

      if (*i == "object") {
        sdf::Object c = parse_object(i);
        if (*i == "[") {
          c = fp(i, this, c);
        } else {
          std::cerr << "error parsing level file: expected \'[\' at start of flags" << std::endl;
          exit(1);
        }
        if (fp.functions.find("moving") != fp.functions.end()) {
          c.velocity = (1.0f / std::any_cast<glm::vec4>(fp.functions["moving"]).w) * (glm::vec3(std::any_cast<glm::vec4>(fp.functions["moving"])) - c.position);
        }
        current_bounds->add_object(c);
      
        fp.clean();
      }

    }

    this->scene.add_volume(current_bounds.value());

  }

  void Level::play(GLFWwindow *window) {

    /* reset renderer state */
    renderer_state.frame_time = 0.0f;
    renderer_state.last_frame = static_cast<float>(glfwGetTime());
    renderer_state.current_frame = static_cast<float>(glfwGetTime());
    renderer_state.total_frames = 0;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader screen("./shader/vertex.glsl", "./shader/frag.glsl");
    Player player(this->spawn);
    this->running = true;

    event::timed_job render_state_update_job(render::update_render_state, -1);
    render_state_update_job.add_to(this->jobs, std::string("update_job"));

    while (this->running) {

      player.update_dir(1, 2);

      event::game::process_input(player, this->scene, window);
      this->scene.update();
      player.update_position(this->scene);

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      screen.use();

      screen.set_matrix<glm::mat4>("view", player.get_view());
      screen.set_matrix<glm::mat4>("projection", player.get_proj());

      this->scene.render(screen);

      event::address_active_jobs(this->jobs);

      std::cout << player.position << std::endl;


      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


  }

  bool is_on_segment(glm::vec3 a, glm::vec3 b, glm::vec3 point) {
    if (glm::distance(a, point) + glm::distance(b, point) == glm::distance(a, b)) {
      return true;
    } else {
      return false;
    }
  }

  glm::vec3 velocity_sampler(glm::vec3 start, glm::vec3 end, glm::vec3 point, glm::vec3 velocity) {
    if (is_on_segment(start, end, point + velocity)) {
      return velocity;
    } else {
      return -velocity;
    }
  }


}
