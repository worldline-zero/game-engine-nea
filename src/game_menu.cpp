#include "../inc/game_menu.hpp"

namespace game {

  gui::Page get_title_screen(gui::GUI *g, GLFWwindow *w) {

    auto exit_button_func = []() -> void { exit(0); };

    auto start_button_func =  std::bind(gui::change_displayed_page, g, "level_selector");
           // the sketchy technique used here is because the function passed to button class has the specific siganture void(void)
           // parameter binding to match the signature is used here (using std::bind) for the purpose of making a function which can
           // dynamically change what the current focused page is in the GUI object whilst also being able to be used as a function bound
           // to a button. a similar thing is used in inc/level.hpp in order to match the signature of a timed_job function

    unsigned int button_width = 40;
    unsigned int button_height = 45;
    unsigned int padding = 10;
    unsigned int exit_button_x = SCREEN_WIDTH - button_width - padding;
    unsigned int exit_button_y = SCREEN_HEIGHT - button_height - padding;
    unsigned int character_size = 40;
   
    std::string start_text = "START!";

    gui::Button exit_button(
        gui::pixel_cast<NDC>(exit_button_x, exit_button_y),
        gui::pixel_cast<SIZE>(button_width, button_height),
        exit_button_func
    );
    gui::Button start_button(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (start_text.size()/2) * character_size - padding, SCREEN_HEIGHT/2 - character_size/2 - padding),
        gui::pixel_cast<SIZE>(start_text.size() * character_size + padding * 2, character_size + 2 * padding),
        start_button_func
    );

    gui::Font font("./res/bitmap_font1.png");

    gui::Label exit_label(
       gui::pixel_cast<NDC>(exit_button_x, exit_button_y + padding/2),
       "X",
       font,
       gui::pixel_cast<SIZE>(character_size, character_size),
       glm::vec4(glm::vec3(0), 1)
    );

    gui::Label start_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (start_text.size()/2) * character_size, SCREEN_HEIGHT/2 - character_size/2),
        start_text,
        font,
        gui::pixel_cast<SIZE>(character_size, character_size),
        glm::vec4(glm::vec3(0), 1)
    );

    gui::Page title_screen;
    title_screen << exit_button << start_button << exit_label << start_label;

    return title_screen;

  }

  gui::Page get_level_selector(gui::GUI *g, GLFWwindow *w, std::string level_dir_path) {

    std::vector<std::string> level_names;

    for (const auto &file : std::filesystem::directory_iterator(level_dir_path)) {
      if (std::filesystem::is_directory(file.path()) == false) {
        level_names.push_back(file.path());
      }
    }

    std::sort(level_names.begin(), level_names.end());

    gui::Page level_selector;
    gui::Font bitmap_font("./res/bitmap_font1.png");


    unsigned int character_width = 20;
    unsigned int button_height = 40;
    unsigned int padding = 10;
    
    unsigned int button_x = padding;
    unsigned int button_y = SCREEN_HEIGHT - button_height - padding - button_height - padding;
    for (const auto &name : level_names) {
      auto current_level_button_func = std::bind([](GLFWwindow *w, gui::GUI *g, std::string name) -> void {
          level::Level current_level(name);
          current_level.play(w, g);
          std::cout << "time taken: " << current_level.time_running << std::endl;
        }, w, g, name);


      std::string cleaned_name = name.substr(name.find_last_of("/") + 1, name.find_last_of(".") - name.find_last_of("/") - 1);
      std::transform(cleaned_name.begin(), cleaned_name.end(), cleaned_name.begin(), toupper);

      unsigned int button_length = (cleaned_name.size() * character_width) + (padding * 2);

      if (button_x + button_length > SCREEN_WIDTH - padding) {
        button_x = padding;
        button_y -= (button_height + padding);
        if (button_y < padding) {
          std::string level_alert_text = "TOO MANY LEVELS!";
          gui::Label level_count_alert(
              gui::pixel_cast<NDC>((SCREEN_WIDTH/2) - 30 * (level_alert_text.size()/2), 10),
              level_alert_text,
              bitmap_font,
              gui::pixel_cast<SIZE>(30, 30),
              glm::vec4(0, 0, 0, 1)
          );
          level_selector << level_count_alert;
          std::cout << "too many levels!" << std::endl;
          break;
        }
      }

      gui::Button current_level_button(
          gui::pixel_cast<NDC>(button_x, button_y),
          gui::pixel_cast<SIZE>(button_length, button_height),
          current_level_button_func
      );

      gui::Label current_level_label(
          gui::pixel_cast<NDC>(button_x + padding, button_y + padding),
          cleaned_name,
          bitmap_font,
          gui::pixel_cast<SIZE>(character_width, button_height - (padding * 2)),
          glm::vec4(glm::vec3(0), 1)
      );

      button_x += button_length + padding;

      level_selector << current_level_button << current_level_label;
      std::cout << name << std::endl;
    }
    
    std::string page_title = "LEVEL SELECTOR";
    gui::Label title(
        gui::pixel_cast<NDC>((SCREEN_WIDTH/2) - 40 * (page_title.size()/2), SCREEN_HEIGHT - 40),
        page_title,
        bitmap_font,
        gui::pixel_cast<SIZE>(40, 40),
        glm::vec4(0, 0, 0, 1)
    );

    level_selector << title;

    std::string back_label_text = "BACK";
    auto back_button_func = std::bind(gui::change_displayed_page, g, "title_screen");
    gui::Button back_button(gui::pixel_cast<NDC>(10, 10), gui::pixel_cast<SIZE>(70, 25), back_button_func);
    gui::Label back_label(gui::pixel_cast<NDC>(15, 15), back_label_text, bitmap_font, gui::pixel_cast<SIZE>(15, 15), glm::vec4(glm::vec3(0), 1));

    level_selector << back_button << back_label;


    return level_selector;

  }

  gui::Page get_fail_screen(gui::GUI *g, GLFWwindow *w) {

    auto accept_button_func = std::bind(gui::change_displayed_page, g, "level_selector");

    unsigned int padding = 10;
    unsigned int character_size = 40;
    std::string fail_text = "YOU FAILED!";
    std::string accept_text = "OK";

    gui::Font bitmap_font("./res/bitmap_font1.png");

    gui::Button accept_button(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (accept_text.size()/2 * character_size) - padding, SCREEN_HEIGHT/2 - character_size/2 - padding),
        gui::pixel_cast<SIZE>(accept_text.size() * character_size + 2 * padding, character_size + 2 * padding),
        accept_button_func
    );

    gui::Label accept_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (accept_text.size()/2) * character_size, SCREEN_HEIGHT/2 - character_size/2),
        accept_text,
        bitmap_font,
        gui::pixel_cast<SIZE>(character_size, character_size),
        glm::vec4(glm::vec3(0), 1)
    );

    gui::Label fail_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (fail_text.size()/2) * character_size, SCREEN_HEIGHT - padding - character_size),
        fail_text,
        bitmap_font,
        gui::pixel_cast<SIZE>(character_size, character_size),
        glm::vec4(0, 0, 0, 1)
    );

    gui::Page fail_page;
    fail_page << accept_button << accept_label << fail_label;

    return fail_page;

  }




  gui::Page get_success_screen(gui::GUI *g, GLFWwindow *w, unsigned int time_taken) {

    auto accept_button_func = std::bind(gui::change_displayed_page, g, "level_selector");

    unsigned int padding = 10;
    unsigned int character_size = 40;
    std::string win_text = "YOU WON!";
    std::string time_text = "TIME: " + std::to_string(time_taken/1000.0f);
    std::string accept_text = "OK";

    gui::Font bitmap_font("./res/bitmap_font1.png");

    gui::Button accept_button(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (accept_text.size()/2 * character_size) - padding, SCREEN_HEIGHT/2 - character_size/2 - padding),
        gui::pixel_cast<SIZE>(accept_text.size() * character_size + 2 * padding, character_size + 2 * padding),
        accept_button_func
    );

    gui::Label accept_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (accept_text.size()/2) * character_size, SCREEN_HEIGHT/2 - character_size/2),
        accept_text,
        bitmap_font,
        gui::pixel_cast<SIZE>(character_size, character_size),
        glm::vec4(glm::vec3(0), 1)
    );

    gui::Label win_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (win_text.size()/2) * character_size, SCREEN_HEIGHT - padding - character_size),
        win_text,
        bitmap_font,
        gui::pixel_cast<SIZE>(character_size, character_size),
        glm::vec4(0, 0, 0, 1)
    );

    gui::Label time_label(
        gui::pixel_cast<NDC>(SCREEN_WIDTH/2 - (time_text.size()/2) * character_size, SCREEN_HEIGHT - 2 * padding - 2 * character_size),
        time_text,
        bitmap_font,
        gui::pixel_cast<SIZE>(character_size, character_size),
        glm::vec4(0, 0, 0, 1)
    );

    gui::Page win_page;
    win_page << accept_button << accept_label << win_label << time_label;

    return win_page;

  }

}
