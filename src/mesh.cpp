#include "../inc/mesh.hpp"

namespace sdf {

  Vertex::Vertex() : position(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), uv(glm::vec2(0.0f)) {}

  Vertex::Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) : position(p), normal(n), uv(t) {}

  Mesh::Mesh(const std::string mesh_path) {
    
    std::ifstream obj_file(mesh_path);

    std::string input;

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<glm::vec2> temp_uvs;

    std::vector<unsigned int> position_indices, normal_indices, uv_indices;

    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

    int face_counter = 0;

    while (obj_file >> input) {

      if (input == "v") {
        
        float p1, p2, p3;
        obj_file >> p1 >> p2 >> p3;
        temp_positions.push_back(glm::vec3(p1, p2, p3));

      } else if (input == "vn") {

        float n1, n2, n3;
        obj_file >> n1 >> n2 >> n3;
        temp_normals.push_back(glm::vec3(n1, n2, n3));

      } else if (input == "vt") {

        float t1, t2;
        obj_file >> t1 >> t2;
        temp_uvs.push_back(glm::vec2(t1, t2));

      } else if (input == "f") {

        for (int i = 1; i<=3; i++) { // triangle so 3 iterations
          std::string vertex;
          obj_file >> vertex;
          boost::char_separator<char> fwd_slash("/");
          tokenizer point_attrs(vertex, fwd_slash);
        
          tokenizer::iterator token_it = point_attrs.begin();
          position_indices.push_back(std::stoi(*token_it));
          token_it++;
          uv_indices.push_back(std::stoi(*token_it));
          token_it++;
          normal_indices.push_back(std::stoi(*token_it));

          face_counter++;
        }

      }
      
    } // end while
    
    for (int i = 0; i<face_counter; i++) {

      this->vertices.push_back(
          Vertex(
            temp_positions[position_indices[i]-1],
            temp_normals[normal_indices[i]-1],
            temp_uvs[uv_indices[i]-1]
            )
          );
    }

    this->generate_VAO();

  }

  void Mesh::generate_VAO() {

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);

  }

  void Mesh::draw() {

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    glBindVertexArray(0);

  }

}
