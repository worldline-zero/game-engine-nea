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

    flags_parser fp;

    sdf::AABB bounds(glm::vec3(0.0f), glm::vec3(10.0f));

    for (std::vector<std::string>::iterator i = tokens_vector.begin(); i!=tokens_vector.end(); i++) {

      /*
      if (*i == "cuboid") {
        sdf::Cuboid c = *parse_cuboid(i);
        c = fp(i, this, c);
        if (fp.functions.find("moving") != fp.functions.end()) {
          c.velocity = (1.0f / std::any_cast<glm::vec4>(fp.functions["moving"]).w) * (glm::vec3(std::any_cast<glm::vec4>(fp.functions["moving"])) - c.center);
        }
        bounds.add_object(c);
      } else if (*i == "sphere") {
        sdf::Sphere c = *parse_sphere(i);
        c = fp(i, this, c);
        if (fp.functions.find("moving") != fp.functions.end()) {
          c.velocity = (1.0f / std::any_cast<glm::vec4>(fp.functions["moving"]).w) * (glm::vec3(std::any_cast<glm::vec4>(fp.functions["moving"])) - c.center);
        }
        bounds.add_object(c);
      } else {
        //std::cout << *i << std::endl;
      } */

      std::cout << *i << std::endl;

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
        bounds.add_object(c);
      
        fp.clean();
      }

    }

    this->scene.add_volume(bounds);

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
