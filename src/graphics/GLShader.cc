/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "GLShader.h"
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"

GLShader::GLShader(GLenum type, const char *path)
    : type_(type),
      path_(path),
      name_(0) {
}

GLShader::~GLShader() {
  if (name_ != 0) {
    glDeleteShader(name_);
  }
}

int GLShader::Compile() {
  name_ = glCreateShader(type_);
  if (name_ == 0) {
    LOGGER.Error("Failed to create shader (type: %d)", type_);
    return -1;
  }

  if (glCompileShaderFile(name_, path_) < 0) {
    LOGGER.Error("Failed to compile shader file (path: %s)", path_);
    glDeleteShader(name_);
    name_ = 0;
    return -1;
  }
  return 0;
}

GLShaderProgram::GLShaderProgram()
    : shaders_(),
      name_(0) {
}

GLShaderProgram::~GLShaderProgram() {
  Clean();
}

void GLShaderProgram::PushShader(const GLShader &shader) {
  shaders_.push_back(shader.name());
}

int GLShaderProgram::Link() {
  name_ = glCreateProgram();
  if (name_ == 0) {
    LOGGER.Error("Failed to create program");
    return -1;
  }

  if (glLinkProgramWithShaders(name_, shaders_) < 0) {
    LOGGER.Error("Failed to link program");
    glDeleteProgram(name_);
    name_ = 0;
    return -1;
  }
  return 0;
}

void GLShaderProgram::Clean() {
  if (name_ != 0) {
    glDeleteProgram(name_);
    name_ = 0;
  }
  shaders_.clear();
}

