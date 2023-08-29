#ifndef RENDERER_STATE_H
#define RENDERER_STATE_H

#include <iostream>
#include <GLFW/glfw3.h>

struct renderer_state_container {
  float frame_time = 0.0f;
  float last_frame = 0.0f;
  float current_frame = 0.0f;
  unsigned int total_frames = 0;
};

namespace render {

  void print_frame_info(unsigned int ct, unsigned int tt);

  void update_render_state(unsigned int ct, unsigned int tt);

}

#endif
