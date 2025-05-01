#pragma once

#include <fstream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Shader {
public:
  unsigned int ID;

  Shader() : ID{0} {}
  Shader(const char *vertexPath, const char *fragmentPath) {
    // open from file
    string vertexCode, fragmentCode;
    ifstream vShaderFile, fShaderFile;

    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      stringstream vShaderStream, fShaderStream;

      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();

      vShaderFile.close();
      fShaderFile.close();

      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (ifstream::failure &e) {
      cerr << "ERROR: Failed to load shader file" << e.what() << "\n";
    }

    // compile shaders
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // clean up
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  void use() { glUseProgram(ID); }

  void setBool(const string &name, bool value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  void setInt(const string &name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const string &name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z,
               float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }

  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }

  // Assignment Overload
  Shader &operator=(const Shader &S) {
    if (this != &S)
      ID = S.ID;
    return *this;
  }

private:
  void checkCompileErrors(unsigned int shader, string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        cerr << "ERROR: SHADER COMPILATION FAILED " << type << "\n"
             << infoLog << "\n";
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        cerr << "ERROR: PROGRAM LINK FAILED " << type << "\n"
             << infoLog << "\n";
      }
    }
  }
};
