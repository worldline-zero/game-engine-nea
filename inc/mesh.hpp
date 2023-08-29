#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "../lib/include/glad/gl.h"

#include <glm/glm.hpp>

#include <boost/tokenizer.hpp>

namespace sdf {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    Vertex();
    Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t);
  };

  class Mesh {

    public:

      std::vector<Vertex> vertices;

      unsigned int VAO;
      unsigned int VBO;

      Mesh() = default;
      
      Mesh(const std::string mesh_path);

      void draw();

      constexpr Mesh &operator=(const Mesh &other) {

        this->vertices = other.vertices;
        this->VAO = other.VAO;
        this->VBO = other.VBO;

        return *this;

      }

    private:

      void generate_VAO();

  };

}

#endif
