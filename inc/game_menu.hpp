#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <algorithm>
#include <filesystem>
#include <type_traits>

#include "../inc/gui.hpp"
#include "../inc/level.hpp"

namespace game {

  gui::Page get_title_screen(gui::GUI *g, GLFWwindow *w);

  gui::Page get_level_selector(gui::GUI *g, GLFWwindow *w, std::string level_dir_path);

  gui::Page get_fail_screen(gui::GUI *g, GLFWwindow *w);

  gui::Page get_success_screen(gui::GUI *g, GLFWwindow *w, unsigned int time_taken);

}

#endif
