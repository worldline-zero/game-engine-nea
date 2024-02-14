#include "../inc/start_opengl.hpp"

namespace loader {

  GLFWwindow* load_gl() {

    if (glfwInit() == GL_FALSE) {
      std::cerr << "error: failed to load glfw \n";
      return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGL((GLADloadfunc)glfwGetProcAddress);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND); // enables transparency (reason for object sorting in bounding.cpp)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE); // makes transparent objects look correct (also is nice for small performance boost)
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // blender triangle winding order is counter clockwise 

    return window;
  }

  void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
  }

}
