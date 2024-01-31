#ifndef SSBO_H
#define SSBO_H

#include <vector>

#include "../lib/include/glad/gl.h"

template <class T>
class SSBO {

  public:

    unsigned int ssbo;
    unsigned int binding_point;
    std::vector<T> data;

    SSBO(std::vector<T> d, unsigned int b);
    
    void update();

};

template <class T>
unsigned int create_ssbo(std::vector<T> data, unsigned int binding_point) {

  unsigned int ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * data.size(), &data[0], GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  return ssbo;

}

template <class T>
SSBO<T>::SSBO(std::vector<T> d, unsigned int b) : binding_point(b), data(d) {

  glGenBuffers(1, &this->ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(T) * this->data.size(), &this->data[0], GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding_point, this->ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

template <class T>
void SSBO<T>::update() {

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
  T *new_data = (T*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
  for (int i = 0; i<this->data.size(); i++) {
    this->data[i] = new_data[i];
  }
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}


#endif 
