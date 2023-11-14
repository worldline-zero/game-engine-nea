#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include <optional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <glm/glm.hpp>

#include "./scene.hpp"


namespace level {

  typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

  glm::vec3 parse_vec3(Tokenizer::iterator &i); // parses a vec3 ( float float float ). assumes typename has already been parsed

  std::unique_ptr<sdf::Cuboid> parse_cuboid(Tokenizer::iterator &i); // parses a cuboid. cuboid name already parsed when called.
                                                                                 // does not parse flags

  std::unique_ptr<sdf::Sphere> parse_sphere(std::ifstream &file);

  std::unique_ptr<sdf::Arbitrary> parse_mesh(std::ifstream &file);

  std::unique_ptr<sdf::AABB> parse_AABB(std::ifstream &file);

  std::unique_ptr<sdf::bounding_sphere> parse_bsphere(std::ifstream &file);

}

#endif
