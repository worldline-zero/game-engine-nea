#include "../inc/level.hpp"

extern struct renderer_state_container renderer_state;

namespace level {

  Level::Level(const std::string level_path) : running(false), time_running(0) {
    std::ifstream file(level_path);
    std::string src((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    boost::char_separator<char> sep("\n\t\a ");
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
    i++;
    this->kill_floor = boost::lexical_cast<float>(*(++i));

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

  void Level::play(GLFWwindow *window, gui::GUI *g) {

    /* reset renderer state */
    renderer_state.frame_time = 0.0f;
    renderer_state.last_frame = static_cast<float>(glfwGetTime());
    renderer_state.current_frame = static_cast<float>(glfwGetTime());
    renderer_state.total_frames = 0;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader object_shader("./shader/object_vertex.glsl", "./shader/object_fragment.glsl");
    Shader light_shader("./shader/light_vertex.glsl", "./shader/light_fragment.glsl");

    std::vector<struct light_info> scene_lights;

    for (const auto &[bid, volume] : this->scene.volumes) {
      for (const auto &[oid, obj] : volume.children) {
        if (obj.light_intensity > 0.0f) {
          struct light_info light;
          light.color_and_intensity = glm::vec4(obj.color.x, obj.color.y, obj.color.z, obj.light_intensity) / glm::vec4(glm::vec3(255.0f), 1.0f);
          light.position = glm::vec4(obj.position.x, obj.position.y, obj.position.z, 1.0f);
          scene_lights.push_back(light);
        }
      }
    }

    const unsigned int light_ssbo_binding_point = 4;
    SSBO<struct light_info> light_ssbo(scene_lights, light_ssbo_binding_point);
    
    Player player(this->spawn);
    this->running = true;
    this->completed = false;

    event::timed_job render_state_update_job(render::update_render_state, -1);
    render_state_update_job.add_to(this->jobs, std::string("update_job"));

    gui::Font bitmap_font("./res/bitmap_font1.png");

    while (this->running) {

      player.update_dir(1, 2);

      event::game::process_input(player, this->scene, window);
      this->scene.update();
      player.update_position(this->scene);
      if (player.position.y < this->kill_floor) {
        break;
      }

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      object_shader.use();

      object_shader.set_matrix<glm::mat4>("view", player.get_view());
      object_shader.set_matrix<glm::mat4>("projection", player.get_proj());
      object_shader.set_vector<glm::vec3>("view_position", player.position);

      light_shader.use();

      light_shader.set_matrix<glm::mat4>("view", player.get_view());
      light_shader.set_matrix<glm::mat4>("projection", player.get_proj());

      this->scene.render(object_shader, light_shader, player.position);

      /*
      gui::Label fps_label(
          gui::pixel_cast<NDC>(0, SCREEN_HEIGHT - character_size),
          "FPS: " + std::to_string(1.0f/renderer_state.frame_time),
          bitmap_font,
          gui::pixel_cast<SIZE>(character_size, character_size)
      );
      fps_label.render();
      */

      render_fps_counter(bitmap_font);
      render_timer(bitmap_font, this->time_running);

      event::address_active_jobs(this->jobs);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (this->completed && this->time_running != 0) {
      *g << std::make_pair("success_screen", game::get_success_screen(g, window, this->time_running));
      gui::change_displayed_page(g, "success_screen");
    } else {
      gui::change_displayed_page(g, "fail_screen");
    }


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

  void render_fps_counter(gui::Font f) {

    const unsigned int fps_character_size = 15;
    gui::Label fps_label(
        gui::pixel_cast<NDC>(0, 0),
        "FPS: " + std::to_string(1.0f/renderer_state.frame_time),
        f,
        gui::pixel_cast<SIZE>(fps_character_size, fps_character_size)
    );
    fps_label.render();

  }

  void render_timer(gui::Font f, unsigned int current_time) {

    const unsigned int timer_character_size = 40;
    std::string time_text = std::to_string(current_time/1000.0f);
    time_text = time_text.substr(0, time_text.find_first_of(".") + 4);
    gui::Label timer_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (time_text.size()/2) * timer_character_size, SCREEN_HEIGHT - timer_character_size),
        time_text,
        f,
        gui::pixel_cast<SIZE>(timer_character_size, timer_character_size)
    );
    timer_label.render();

  }


}
