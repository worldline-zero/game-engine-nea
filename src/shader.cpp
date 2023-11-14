#include "../inc/shader.hpp"
#include "../inc/scene.hpp"

Shader::Shader(const char *compute_path) {

  std::string compute_src;
  std::ifstream compute_file(compute_path);

  if (compute_file) {
    std::stringstream compute_stream;
    compute_stream << compute_file.rdbuf();
    compute_file.close();
    compute_src = compute_stream.str();
  } else {
    std::cerr << "error opening file: " << compute_path << std::endl;
    exit(1);
  }

  const char *compute_code = compute_src.c_str();

  unsigned int compute;

  compute = glCreateShader(GL_COMPUTE_SHADER);
  glShaderSource(compute, 1, &compute_code, NULL);
  glCompileShader(compute);
  check_errors(compute, "compute");

  ID = glCreateProgram();
  glAttachShader(ID, compute);
  glLinkProgram(ID);
  check_errors(ID, "program");

  glDeleteShader(compute);

}

Shader::Shader(const char *vertex_path, const char *frag_path) {

  std::string vertex_src;
  std::ifstream vertex_file(vertex_path);

  if (vertex_file) {
    std::stringstream vertex_stream;
    vertex_stream << vertex_file.rdbuf();
    vertex_file.close();
    vertex_src = vertex_stream.str();
  } else {
    std::cerr << "error opening file: " << vertex_path << std::endl;
    exit(1);
  }

  std::string frag_src;
  std::ifstream frag_file(frag_path);

  if (frag_file) {
    std::stringstream frag_stream;
    frag_stream << frag_file.rdbuf();
    frag_file.close();
    frag_src = frag_stream.str();
  } else {
    std::cerr << "error opening file: " << frag_path << std::endl;
    exit(1);
  }

  const char *vertex_code = vertex_src.c_str();

  unsigned int vertex;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_code, NULL);
  glCompileShader(vertex);
  check_errors(vertex, "vertex");

  const char *frag_code = frag_src.c_str();

  unsigned int frag;

  frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_code, NULL);
  glCompileShader(frag);
  check_errors(frag, "frag");


  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, frag);
  glLinkProgram(ID);
  check_errors(ID, "program");

  glDeleteShader(vertex);
  glDeleteShader(frag);

}

void Shader::use() {
  glUseProgram(ID);
}



void Shader::check_errors(GLuint shader, std::string shader_type) {
  int success;
  char infolog[1024];
  if (shader_type == "program") {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infolog);
      std::cout << "program linking error of type: " << shader_type << "\n" << infolog << std::endl;
      exit(1);
    }
  } else {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infolog);
      std::cout << "shader compile error of type: " << shader_type << "\n" << infolog << std::endl;
      exit(1);
    }
  }
}


