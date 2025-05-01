#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "text_render.h"

class WindowManager {
public:
    WindowManager(int width, int height, const char* title) : _windowWidth(width), _windowHeight(height) {
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      _window = glfwCreateWindow(width, height, title, NULL, NULL);
      glfwMakeContextCurrent(_window);
      gladLoadGL((GLADloadfunc)glfwGetProcAddress);

      _textRender = new TextRenderer("../resources/fonts/JetBrainsMono-Regular.ttf");
      _textShader = Shader("../resources/shaders/text.vert", "../resources/shaders/text.frag");
    }

    WindowManager(int width, int height) : WindowManager(width, height, "Window") {}
    WindowManager() : WindowManager(800, 600) {}

    ~WindowManager() {
      glfwDestroyWindow(_window);
      glfwTerminate();
    }

    //
    // Main Loop
    //
    void updateWindowSize() {
      glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
    }

    void update() {
      glfwSwapBuffers(_window);
      glfwPollEvents();
    }

    bool shouldClose() {
      return glfwWindowShouldClose(_window);
    }

    //
    // CLEAR COLOR
    //
    void setClearColor(glm::vec3 color) {
      glClearColor(color.r, color.g, color.b, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    void setClearColor(glm::vec4 color) {
      glClearColor(color.r, color.g, color.b, color.a);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    void setClearColor(glm::vec3 color, float alpha) {
      glClearColor(color.r, color.g, color.b, alpha);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    void setClearColor(float r, float g, float b, float a) {
      glClearColor(r, g, b, a);
      glClear(GL_COLOR_BUFFER_BIT);
    }


  //
  // FONT RENDERING
  //
  void renderText(const std::string& text, float x, float y, float scale, glm::vec3 color) {
    _textRender->Render(&_textShader, text, x, y, scale, color, (float)_windowWidth, (float)_windowHeight);
  }

  void renderText(const char* text, float x, float y, float scale, glm::vec3 color) {
    _textRender->Render(&_textShader, std::string(text), x, y, scale, color, (float)_windowWidth, (float)_windowHeight);
  }

private:
    GLFWwindow* _window;
    Shader _textShader;
    TextRenderer* _textRender;
    int _windowWidth;
    int _windowHeight;
};
