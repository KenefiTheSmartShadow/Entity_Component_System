#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#include "shader.h"

class TextRenderer {
  GLuint VAO, VBO;

  struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
  };

public:
  TextRenderer(const char *font) {
    initialize();
    createVertexAttributes();
  }

  TextRenderer() {}

  ~TextRenderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
  }

  // Text Render Func
  void Render(Shader *shader, std::string text, float x, float y, float scale,
              glm::vec3 color, float width, float height) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();
    shader->setVec3("textColor", color);

    glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
    shader->setMat4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
      Character ch = _characters[*c];

      GLfloat xpos = x + ch.bearing.x * scale;
      GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

      GLfloat w = ch.size.x * scale;
      GLfloat h = ch.size.y * scale;

      GLfloat vertices[6][4] = {
          {xpos, ypos + h, 0.0f, 0.0f}, {xpos, ypos, 0.0f, 1.0f},
          {xpos + w, ypos, 1.0f, 1.0f}, {xpos, ypos + h, 0.0f, 0.0f},
          {xpos + w, ypos, 1.0f, 1.0f}, {xpos + w, ypos + h, 1.0f, 0.0f}};

      glBindTexture(GL_TEXTURE_2D, ch.textureID);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      x += (ch.advance >> 6) *
           scale; // Bit shift by 6 to get the correct advance value
    }
  }

  TextRenderer &operator=(const TextRenderer &other) {
    _characters = other._characters;
    VAO = other.VAO;
    VBO = other.VBO;
    return *this;
  }

private:
  std::map<GLchar, Character> _characters;

  void initialize() {
    // Initialize FreeType
    FT_Library ftLibrary;
    auto error = FT_Init_FreeType(&ftLibrary);
    if (error) {
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n"
                << std::endl;
    }

    // Load a font
    FT_Face ftFace;

    error = FT_New_Face(
        ftLibrary, "../resources/fonts/JetBrainsMono-Regular.ttf", 0, &ftFace);
    if (error == FT_Err_Unknown_File_Format) {
      std::cout
          << "ERROR::FREETYPE: Could not load font\nFile type not supported\n"
          << std::endl;
    } else if (error) {
      std::cout << "ERROR::FREETYPE: Could not open font\n" << std::endl;
    }

    // Set Pixel Size
    FT_Set_Pixel_Sizes(ftFace, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (GLubyte i = 0; i < 128; i++) {
      if (FT_Load_Char(ftFace, i, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYPE: Could not load Glyph '" << i << "'\n"
                  << std::endl;
        continue;
      }

      // Generate texture
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ftFace->glyph->bitmap.width,
                   ftFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                   ftFace->glyph->bitmap.buffer);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Store character for later use
      Character character = {
          texture,
          glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
          glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
          (GLuint)ftFace->glyph->advance.x};
      _characters.insert(std::pair<char, Character>(i, character));
    }

    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLibrary);
  }

  void createVertexAttributes() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
};
