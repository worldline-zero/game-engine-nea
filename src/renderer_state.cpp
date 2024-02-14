#include "../inc/renderer_state.hpp"

extern struct renderer_state_container renderer_state; // contains all the frame information (extern so it is visible to other source files)

renderer_state_container renderer_state;

namespace render {

  void print_frame_info(unsigned int ct, unsigned int tt) { // for debugging, irrelevent after on screen fps counter implemented
    std::cout << "fps: " << 1.0f/renderer_state.frame_time << "\n";
  }

  // literally the most important thing
  void update_render_state(unsigned int ct, unsigned int tt) {
    renderer_state.current_frame = glfwGetTime();
    renderer_state.frame_time = renderer_state.current_frame - renderer_state.last_frame;
    renderer_state.last_frame = renderer_state.current_frame;
    renderer_state.total_frames++;
  }

}
