#ifndef LEVEL_H
#define LEVEL_H

#include <algorithm>
#include <any>
#include <vector>
#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>
#include <memory>
#include <functional>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "../inc/scene.hpp"
#include "../inc/physics.hpp"
#include "../inc/events.hpp"
#include "../inc/ssbo.hpp"
#include "../inc/game_menu.hpp"

extern struct renderer_state_container renderer_state;


namespace level {

  typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

  struct light_info {
    glm::vec4 color_and_intensity;
    glm::vec4 position;
  };

  class Level {

    public:

      sdf::Scene scene;

      unsigned int start_ID;
      unsigned int end_ID;

      glm::vec3 spawn;
      float kill_floor;

      bool running;
      bool completed;

      unsigned int time_running;
      unsigned int time_limit;

      std::map<std::string, event::timed_job> jobs;

      Level(const std::string level_path);

      void play(GLFWwindow *window, gui::GUI *g);

  };

  glm::vec3 parse_vec3(std::vector<std::string>::iterator &i);

  glm::uvec4 parse_color(std::vector<std::string>::iterator &i);

  sdf::Object parse_object(std::vector<std::string>::iterator &i);

  sdf::AABB parse_AABB(std::vector<std::string>::iterator &i);

  void include_file(std::vector<std::string> &original_file, std::vector<std::string>::iterator &i, std::string include_path);

  bool is_on_segment(glm::vec3 a, glm::vec3 b, glm::vec3 point);

  glm::vec3 velocity_sampler(glm::vec3 start, glm::vec3 end, glm::vec3 point, glm::vec3 velocity);

  class flags_parser {

    public:

      std::map<std::string, std::any> functions;

      sdf::Object &operator()(std::vector<std::string>::iterator &i, Level *l, sdf::Object &object) {

        while (*(++i) != "]") {


          if (*i == "start") {
            this->functions.insert(std::make_pair("start", 0u));
            auto collision_behaviour = [](physics::collision_info cb, sdf::Object &object, Level *l) {
              auto temp_opt_timed_job =  
              [](unsigned int ct, unsigned int tt, Level *l) {
                std::cout << ct << std::endl;
                l->time_running = ct;
                if (ct > l->time_limit) {
                  l->running = false;
                }
              };
              object.opt_timed_job = event::timed_job(std::bind(temp_opt_timed_job, std::placeholders::_1, std::placeholders::_2, l), -1);
              object.opt_timed_job.value().add_to(l->jobs, std::to_string(object.ID) + "start_func");
            };
            object.collision_behaviour = std::bind(collision_behaviour, std::placeholders::_1, object, l);
          }


          if (*i == "end") {
            this->functions.insert(std::make_pair("end", 0u));
            auto collision_behaviour = [](physics::collision_info cb, Level *l) {
              l->running = false;
              l->completed = true;
            };
            object.collision_behaviour = std::bind(collision_behaviour, std::placeholders::_1, l);
          }


          if (*i == "moving") {
            if (*(++i) != "vec3") {
              std::cerr << "error parsing level file: expected vec3" << std::endl;
              std::cout << *i << std::endl;
              exit(1);
            }
            glm::vec3 new_pos = parse_vec3(i);
            float d;
            d = boost::lexical_cast<float>(*(++i));
            this->functions.insert(std::make_pair("moving", glm::vec4(new_pos, d)));

            unsigned int interval_ms = d * 1000;
            glm::vec3 start = object.position;
            auto job = [interval_ms, start, d, new_pos](unsigned int ct, unsigned int tt, Level *l, unsigned int ID) {
              for (auto &[vid, vol]:l->scene.volumes) {
                for (auto &[oid, obj]:vol.children) {
                  if (obj.ID == ID) {
                    unsigned int intervals_passed = ct / interval_ms;
                    if (intervals_passed % 2 == 0) {
                      obj.velocity = (new_pos - start) / d;
                    } else {
                      obj.velocity = (start - new_pos) / d;
                    }
                    //obj.velocity = static_cast<float>(pow(-1, intervals_passed)) * obj.velocity;
                  }
                }
              }
            };

            event::timed_job move_job(std::bind(job, std::placeholders::_1, std::placeholders::_2, l, object.ID), -1);
            move_job.add_to(l->jobs, std::to_string(object.ID) + "move_func");

          }


          if (*i == "rotate") {
            if (*(++i) != "vec3") {
              std::cerr << "error parsing level file: expected vec3" << std::endl;
              exit(1);
            }
            glm::vec3 new_rotation = parse_vec3(i);
            float angle_per_frame = boost::lexical_cast<float>(*(++i));

            auto job = [new_rotation, angle_per_frame](unsigned int ct, unsigned int tt, Level *l, unsigned int ID) {
              for (auto &[vid, vol]:l->scene.volumes) {
                for (auto &[oid, obj]:vol.children) {
                  if (obj.ID == ID) {
                    obj.rotation = new_rotation;
                    obj.angle += angle_per_frame;
                    obj.rotational_velocity = glm::radians(angle_per_frame) / renderer_state.frame_time;
                    obj.construct_matrix();
                  }
                }
              }
            };

            event::timed_job rotation_job(std::bind(job, std::placeholders::_1, std::placeholders::_2, l, object.ID), -1);
            rotation_job.add_to(l->jobs, std::to_string(object.ID) + "rotation_func");
            
          }


          if (*i == "accelerate") {
            if (*(++i) != "vec3") {
              std::cerr << "error parsing level file: expected vec3" << std::endl;
              std::cerr << "got: " << *i << std::endl;
              exit(1);
            } else {
              glm::vec3 acceleration_factor = parse_vec3(i);
              object.acceleration = acceleration_factor;
            }
          }


          if (*i == "light") {
            float intensity = 0.0f;
            try {
              intensity = boost::lexical_cast<float>(*(++i));
            } catch (...) {
              std::cerr << "error parsing level file: invalid value" << std::endl;
              std::cerr << "expected float, got: " << *i << std::endl;
              exit(1);
            }
            object.light_intensity = intensity;
          }


        }
        return object;
      }

      void clean();

  };

  void render_fps_counter(gui::Font f);

  void render_timer(gui::Font f, unsigned int current_time);

}

#endif 
