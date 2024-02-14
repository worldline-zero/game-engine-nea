#ifndef GUI_H
#define GUI_H

#include <array>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../inc/shader.hpp"
#include "../inc/texture.hpp"
#include "../inc/start_opengl.hpp"

namespace gui {

  struct char_vertex {
    glm::vec2 position;
    glm::vec2 tex_position;
  };

#define SIZE 0
#define NDC 1

  template <unsigned int T> // handy system to convert between pixels and NDC / size
  glm::vec2 pixel_cast(unsigned int x, unsigned int y) {

    glm::vec2 pixels((float)x, (float)y);
    pixels = pixels / glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT);
    pixels = (pixels * glm::vec2(2.0f)) - glm::vec2(T);
    return pixels;

  }

  glm::vec2 pixel_to_NDC(unsigned int x, unsigned int y);

  glm::vec2 pixel_to_font_size(unsigned int x, unsigned int y);

  glm::vec2 get_cursor_position(GLFWwindow *window);

  class Font {

    public:

      std::map<char, glm::vec2> positions;
      std::string font_path;
      Shader font_shader;
      unsigned int texture;

      Font(std::string bitmap);

      std::array<char_vertex, 4> get_char_vertex_data(glm::vec2 screen_position, char character, glm::vec2 font_size);

      unsigned int get_vao(glm::vec2 screen_position, char character, glm::vec2 font_size);

  };


  class Label {

    public:

      glm::vec2 position;
      std::string text;
      Font font;
      glm::vec2 font_size; // width and height of each character
      glm::vec4 color;
      std::vector<unsigned int> character_VAOs;
      
      Label(glm::vec2 p, std::string t, Font f, glm::vec2 fs, glm::vec4 fc);

      void render();

  };

  class Button {

    public:

      glm::vec2 position;
      glm::vec2 size;
      std::function<void()> button_func;
      unsigned int VAO, VBO;
      Shader button_shader;

      Button(glm::vec2 p, glm::vec2 s, std::function<void()> f);

      void render();

      bool contains(glm::vec2 current_cursor_position);

  };

  class Page {

    public:

      std::vector<Label> labels;
      std::vector<Button> buttons;

      Page();

      void render();

      Page &operator<<(const Label l); // add label

      Page &operator<<(const Button b); // add button

  };

  class GUI {

    public:

      std::map<std::string, Page> pages;

      std::string current_page;

      GUI(std::string start_window);

      void render();

      GUI &operator<<(const std::pair<std::string, Page> p);

  };

  void change_displayed_page(GUI *g, std::string new_page_name); // function seperate from GUI class to allow for parameter binding


}


#endif
