#ifndef LEVEL_H
#define LEVEL_H

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

#include "./scene.hpp"
#include "./physics.hpp"
#include "./events.hpp"


namespace level {

  typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

  class Level {

    public:

      sdf::Scene scene;

      unsigned int start_ID;
      unsigned int end_ID;

      glm::vec3 spawn;

      bool running;

      std::map<std::string, event::timed_job> jobs;

      Level(const std::string level_path);

  };

  glm::vec3 parse_vec3(Tokenizer::iterator &i); // parses a vec3 ( float float float ). assumes typename has already been parsed

//  std::unique_ptr<sdf::Cuboid> parse_cuboid(Tokenizer::iterator &i); // parses a cuboid. cuboid name already parsed when called.

//  std::unique_ptr<sdf::Sphere> parse_sphere(Tokenizer::iterator &i);

  sdf::Object parse_object(Tokenizer::iterator &i);

  sdf::AABB parse_AABB(Tokenizer::iterator &i);

  bool is_on_segment(glm::vec3 a, glm::vec3 b, glm::vec3 point);

  glm::vec3 velocity_sampler(glm::vec3 start, glm::vec3 end, glm::vec3 point, glm::vec3 velocity);

  class flags_parser {

    public:

      std::map<std::string, std::any> functions;

      sdf::Object &operator()(Tokenizer::iterator &i, Level *l, sdf::Object &object) {
        while (*(++i) != "]") {
          if (*i == "start") {
            this->functions.insert(std::make_pair("start", 0u));
            auto collision_behaviour = [](physics::collision_info cb, sdf::Object &object, Level *l) {
              auto temp_opt_timed_job =  
              [](unsigned int ct, unsigned int tt, Level *l) {
                std::cout << ct<< std::endl;
                if (ct > 5000) {
                  l->running = false;
                }
              };
              object.opt_timed_job = event::timed_job(std::bind(temp_opt_timed_job, std::placeholders::_1, std::placeholders::_2, l), 5000);
              object.opt_timed_job.value().add_to(l->jobs, std::to_string(object.ID) + "start_func");
            };
            object.collision_behaviour = std::bind(collision_behaviour, std::placeholders::_1, object, l);
          }
          if (*i == "end") {
            this->functions.insert(std::make_pair("end", 0u));
            auto collision_behaviour = [](physics::collision_info cb, Level *l) {
              l->running = false;
            };
            object.collision_behaviour = std::bind(collision_behaviour, std::placeholders::_1, l);
          }
          if (*i == "moving") {
            i++;
            glm::vec3 new_pos = parse_vec3(i);
            float d;
            d = boost::lexical_cast<float>(*(++i));
            this->functions.insert(std::make_pair("moving", glm::vec4(new_pos, d)));

            unsigned int interval = d * 1000;
            auto job = [interval](unsigned int ct, unsigned int tt, Level *l, unsigned int ID) {
              for (auto &[vid, vol]:l->scene.volumes) {
                for (auto &[oid, obj]:vol.children) {
                  if (obj.ID == ID) {
                    unsigned int intervals_passed = ct / interval;
                    obj.velocity = static_cast<float>(pow(-1, intervals_passed)) * obj.velocity;
                  }
                }
              }
            };

            event::timed_job move_job(std::bind(job, std::placeholders::_1, std::placeholders::_2, l, object.ID), -1);
            move_job.add_to(l->jobs, std::to_string(object.ID) + "move_func");

          }
        }
        return object;
      }

      void clean();

  };

}

#endif 
