#include "../inc/level.hpp"

#define VEC3_ERROR "error parsing level file: expected vec3"

namespace boost {

  template<>
  bool lexical_cast<bool, std::string>(const std::string &s) {
    if (s != "true" && s != "false") {
      boost::throw_exception(boost::bad_lexical_cast());
    }
    std::stringstream ss(s);
    bool b;
    ss >> std::boolalpha >> b;
    return b;
  }

}

namespace level {


  void flags_parser::clean() {
    this->functions.clear();
  }
  
  glm::vec3 parse_vec3(std::vector<std::string>::iterator &i) {
    float x, y, z;
    x = boost::lexical_cast<float>(*(++i));
    y = boost::lexical_cast<float>(*(++i));
    z = boost::lexical_cast<float>(*(++i));
    return glm::vec3(x, y, z);
  }

  glm::uvec4 parse_color(std::vector<std::string>::iterator &i) {
    unsigned int r, g, b, a;
    r = boost::lexical_cast<float>(*(++i));
    g = boost::lexical_cast<float>(*(++i));
    b = boost::lexical_cast<float>(*(++i));
    a = boost::lexical_cast<float>(*(++i));
    return glm::uvec4(r, g, b, a);
  }


  sdf::Object parse_object(std::vector<std::string>::iterator &i) {

    std::string s;

    glm::vec3 position, scale, rotation;
    float angle;
    bool solid;
    sdf::Mesh mesh;
    glm::vec4 obj_color;

    std::string object_name = *(++i);

    if (object_name == "{") {
      std::cerr << "\'{\' is not a valid object name" << std::endl;
      exit(1);
    }
    
    i++; // paren
    
    s = *(++i);
    if (s != "vec3") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << VEC3_ERROR << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        position = parse_vec3(i);
      } catch (...) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }

    s = *(++i);
    if (s != "vec3") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << VEC3_ERROR << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        scale = parse_vec3(i);
      } catch (...) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }

    s = *(++i);
    if (s != "vec3") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << VEC3_ERROR << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        rotation = parse_vec3(i);
      } catch (...) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }

    s = *(++i);
    if (s != "float") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << "error parsing level file: expected float" << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        angle = boost::lexical_cast<float>(*(++i));
      } catch (...) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }

    s = *(++i);
    if (s != "path") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << "error parsing level file: expected file path" << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      std::string filepath = *(++i);
      if (!std::fopen(filepath.c_str(), "r")) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid mesh file path" << std::endl;
        exit(1);
      }
      mesh = sdf::Mesh(filepath);
    }

    s = *(++i);
    if (s != "color") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << "error parsing level file: expected color" << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        obj_color = parse_color(i);
      } catch (...) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }


    s = *(++i);
    if (s != "solid") {
      std::cerr << "in object: " << object_name << std::endl;
      std::cerr << "error parsing level file: expected solid indicator" << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        solid = boost::lexical_cast<bool>(*(++i));
      } catch (...) {
        std::cerr << "in object: " << object_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }
    
    sdf::Object obj(mesh, position, scale, rotation, angle);
    obj.solid = solid;
    obj.color = obj_color;

    i++;
    i++; // closing paren

    return obj;

  }

  sdf::AABB parse_AABB(std::vector<std::string>::iterator &i) {

    std::string aabb_name = *(++i);
    glm::vec3 position, dimensions;
    std::string s;
    
    s = *(++i);
    if (s != "vec3") {
      std::cerr << "in AABB declaration: " << aabb_name << std::endl;
      std::cerr << VEC3_ERROR << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        position = parse_vec3(i);
      } catch (...) {
        std::cerr << "in AABB declaration: " << aabb_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }

    s = *(++i);
    if (s != "vec3") {
      std::cerr << "in AABB declaration: " << aabb_name << std::endl;
      std::cerr << VEC3_ERROR << std::endl;
      std::cerr << "got: " << s << std::endl;
      exit(1);
    } else {
      try {
        dimensions = parse_vec3(i);
      } catch (...) {
        std::cerr << "in AABB declaration: " << aabb_name << std::endl;
        std::cerr << "error parsing level file: invalid value" << std::endl;
        std::cerr << "value in question: " << *i << std::endl;
        exit(1);
      }
    }

    sdf::AABB aabb(position, dimensions);

    return aabb;

  }

  void include_file(std::vector<std::string> &original_file, std::vector<std::string>::iterator &i, std::string include_path) {

    std::ifstream file(include_path);
    std::string src((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    boost::char_separator<char> sep("\n\t\a ");
    Tokenizer tokens(src, sep);

    std::vector<std::string> tokens_vector;
    
    for (Tokenizer::iterator i = tokens.begin(); i!=tokens.end(); i++) {
      tokens_vector.push_back(*i);
    }

    int old_index = original_file.size() - 1;

    original_file.insert(i + 1, tokens_vector.begin(), tokens_vector.end());

    i = original_file.begin() + old_index;


  }

}
