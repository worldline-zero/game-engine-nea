#include "../inc/level.hpp"

#define VEC3_ERROR "error parsing level file: expected vec3"

namespace boost {

  template<>
  bool lexical_cast<bool, std::string>(const std::string &s) {
    std::stringstream ss(s);
    bool b;
    ss >> std::boolalpha >> b;
    return b;
  }

}

namespace level {

  /*
  void flags_parser::operator()(Tokenizer::iterator &i, Level *l) {
    while (*(++i) != "]") {
      if (*i == "start") {
        this->functions.insert(std::make_pair("start", 0u));
      } else if (*i == "end") {
        this->functions.insert(std::make_pair("end", 0u));
      } else if (*i == "moving") {
        i++;
        glm::vec3 new_pos = parse_vec3(i);
        float d = boost::lexical_cast<float>(*(++i));
        this->functions.insert(std::make_pair("moving", glm::vec4(new_pos, d)));
      }
    }
  }
  */

  void flags_parser::clean() {
    this->functions.clear();
  }
  
  glm::vec3 parse_vec3(Tokenizer::iterator &i) {
    float x, y, z;
    x = boost::lexical_cast<float>(*(++i));
    y = boost::lexical_cast<float>(*(++i));
    z = boost::lexical_cast<float>(*(++i));
    return glm::vec3(x, y, z);
  }

  /*

  std::unique_ptr<sdf::Cuboid> parse_cuboid(Tokenizer::iterator &i) {
    
    std::string paren; // characters such as parens and semi colons

    paren = *(++i); // open bracket
    //std::cout << paren << std::endl;
    std::string identifier;

    glm::vec3 half_d, pos, rotation;

    identifier = *(++i);
    //std::cout << identifier << std::endl;
    if (identifier == "vec3") {
      half_d = parse_vec3(i);
      //std::cout << half_d << std::endl;
    } else {
      std::cerr << VEC3_ERROR << std::endl;
      exit(1);
    }

    identifier = *(++i);
    //std::cout << identifier << std::endl;
    if (identifier == "vec3") {
      pos = parse_vec3(i);
      //std::cout << pos << std::endl;
    } else {
      std::cerr << VEC3_ERROR << std::endl;
      exit(1);
    }

    identifier = *(++i);
    //std::cout << identifier << std::endl;
    if (identifier == "vec3") {
      rotation = parse_vec3(i);
      //std::cout << rotation << std::endl;
    } else {
      std::cerr << VEC3_ERROR << std::endl;
      exit(1);
    }

    float angle;
    angle = boost::lexical_cast<float>(*(++i));
    //std::cout << angle << std::endl;

    bool solid;
    solid = boost::lexical_cast<bool>(*(++i));
    //std::cout << solid << std::endl;
    paren = *(++i); // close bracket
    //std::cout << paren << std::endl;

    sdf::Cuboid c(half_d, pos, rotation, angle);

    std::unique_ptr<sdf::Cuboid> cuboid_p = std::make_unique<sdf::Cuboid>(c);

    cuboid_p->solid = solid;

    return cuboid_p;

  }

  std::unique_ptr<sdf::Sphere> parse_sphere(Tokenizer::iterator &i) {

    std::string paren;

    paren = *(++i);

    std::string identifier;

    glm::vec3 position;

    identifier = *(++i);
    if (identifier == "vec3") {
      position = parse_vec3(i);
    } else {
      std::cerr << VEC3_ERROR << std::endl;
      exit(1);
    }

    float radius = boost::lexical_cast<float>(*(++i));
    std::string solid_s = *(++i);
    bool solid;
    if (solid_s == "true") {
      solid = true;
    } else {
      solid = false;
    }

    paren = *(++i);

    sdf::Sphere s(radius, position);
    std::unique_ptr<sdf::Sphere> sphere_p = std::make_unique<sdf::Sphere>(s);
    sphere_p->solid = solid;

    return sphere_p;
  
  }
*/

  sdf::Object parse_object(Tokenizer::iterator &i) {

    return sdf::Object(sdf::Mesh("/home/charlotte/usr/dev/game-engine-nea/res/cube.obj"), glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(1.0f), glm::vec3(1.0f), 0.0f);

  }

}
