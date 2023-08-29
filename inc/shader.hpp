#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <iostream>

#include "../lib/include/glad/gl.h"

class Shader {

  public:

  unsigned int ID;

  Shader(const char *compute_path);

  Shader(const char *vertex_path, const char *frag_path);

  void use();

  void check_errors(GLuint shader, std::string shader_type);

  template <typename T>
  void set_uniform(const std::string &name, const T &value) {
    if (std::is_floating_point<T>::value) {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    } else {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
  }

  template <class T>
  void set_vector(const std::string &name, const T &value) {
    switch (sizeof(T) / sizeof(float)) {
      case 2:
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        break;
      case 3:
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        break;
      case 4:
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        break;
    }
  }

  template <class T>
  void set_matrix(const std::string &name, const T &value) {
    switch (sizeof(T) / sizeof(float)) {
      case 4:
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        break;
      case 9:
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        break;
      case 16:
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
        break;
    }
  }

};

#endif
