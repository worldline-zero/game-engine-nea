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

  typedef ::sdf::Cuboid start_zone;
  typedef ::sdf::Cuboid end_zone;

  typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

  class Level {

    public:

      sdf::Scene scene;

      unsigned int start_ID;
      unsigned int end_ID;

      glm::vec3 spawn;

      bool running;

      std::vector<event::timed_job> jobs;

      Level(const std::string level_path);

  };

  glm::vec3 parse_vec3(Tokenizer::iterator &i); // parses a vec3 ( float float float ). assumes typename has already been parsed

  std::unique_ptr<sdf::Cuboid> parse_cuboid(Tokenizer::iterator &i); // parses a cuboid. cuboid name already parsed when called.
                                                                                 // does not parse flags

  std::unique_ptr<sdf::Sphere> parse_sphere(Tokenizer::iterator &i);

  std::unique_ptr<sdf::Arbitrary> parse_mesh(Tokenizer::iterator &i);

  std::unique_ptr<sdf::AABB> parse_AABB(Tokenizer::iterator &i);

  std::unique_ptr<sdf::bounding_sphere> parse_bsphere(Tokenizer::iterator &i);

  class flags_parser {

    public:

      std::map<std::string, std::any> functions;

      template <typename T>
      T &operator()(Tokenizer::iterator &i, Level *l, T &object) {
        while (*(++i) != "]") {
          if (*i == "start") {
            this->functions.insert(std::make_pair("start", 0u));
            auto collision_behaviour = [](physics::collision_info cb, T &object, Level *l) {
              if (object.collided == false) {
                auto temp_opt_timed_job =  
                [](unsigned int ct, unsigned int tt, Level *l) {
                  std::cout << ct<< std::endl;
                  if (ct > 5000) {
                    l->running = false;
                  }
                };
                object.opt_timed_job = event::timed_job(std::bind(temp_opt_timed_job, std::placeholders::_1, std::placeholders::_2, l), 5000);
                object.opt_timed_job.value().add_to(l->jobs);
              }
            };
            object.collision_behaviour = std::bind(collision_behaviour, std::placeholders::_1, object, l);
          }
          if (*i == "end") {
            this->functions.insert(std::make_pair("end", 0u));
          }
          if (*i == "moving") {
            i++;
            glm::vec3 new_pos = parse_vec3(i);
            float d = boost::lexical_cast<float>(*(++i));
            this->functions.insert(std::make_pair("moving", glm::vec4(new_pos, d)));
          }
        }
        return object;
      }

      void clean();

  };

}

#endif 
