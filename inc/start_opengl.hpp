#include "../lib/include/glad/gl.h"

#include <GLFW/glfw3.h>

#include <iostream>

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f

namespace loader {

  GLFWwindow* load_gl();

  void framebuffer_size_callback(GLFWwindow *win, int w, int h);

}
