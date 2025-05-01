#include <iostream>

#include <glm/glm.hpp>

#include "window_manager.h"

int main() {
  WindowManager window_manager;

  while (!window_manager.shouldClose()) {
    window_manager.updateWindowSize();
    window_manager.setClearColor(glm::vec4(0.0, 0.1, 0.2, 1.0));
    window_manager.renderText("Hello, World!", 100.f, 100.f, 1.0f,
                              glm::vec3(1.0f, 1.0f, 1.0f));
    window_manager.update();
  }

  return 0;
}
