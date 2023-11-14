#include "../inc/level.hpp"

namespace level {

  Level::Level(const std::string level_path) : running(false) {
    std::ifstream file(level_path);
    std::string src((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    boost::char_separator<char> sep("\n ");
    Tokenizer tokens(src, sep);
    std::string recon = "";

    flags_parser fp;

    sdf::AABB bounds;

    for (Tokenizer::iterator i = tokens.begin(); i!=tokens.end(); i++) {

      if (*i == "cuboid") {
        sdf::Cuboid c = *parse_cuboid(i);
        c = fp(i, this, c);
        bounds.add_object(c);
      } else if (*i == "sphere") {
        sdf::Sphere c = *parse_sphere(i);
        c = fp(i, this, c);
        bounds.add_object(c);
      } else {
        //std::cout << *i << std::endl;
      }


    }

    this->scene.add_volume(bounds);

  }

}
