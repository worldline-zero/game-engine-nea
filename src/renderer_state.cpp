#include "../inc/renderer_state.hpp"

extern struct renderer_state_container renderer_state;

renderer_state_container renderer_state;

namespace render {

  void print_frame_info(unsigned int ct, unsigned int tt) {
    std::cout << "fps: " << 1.0f/renderer_state.frame_time << "\n";
  }

  void update_render_state(unsigned int ct, unsigned int tt) {
    renderer_state.current_frame = glfwGetTime();
    renderer_state.frame_time = renderer_state.current_frame - renderer_state.last_frame;
    renderer_state.last_frame = renderer_state.current_frame;
    renderer_state.total_frames++;
  }

}
