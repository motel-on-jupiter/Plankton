/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "Shader.h"
#include "util/wrapper/glgraphics_wrap.h"
#include "util/logging/Logger.h"

Shader::Shader(GLenum type, const char *path)
    : type_(type),
      path_(path),
      name_(0) {
}

Shader::~Shader() {
  if (name_ != 0) {
    glDeleteShader(name_);
  }
}

int Shader::Compile() {
  name_ = glCreateShader(type_);
  if (name_ == 0) {
    LOGGER.Error("Failed to create shader");
    return -1;
  }

  if (glCompileShaderFile(name_, path_) < 0) {
    LOGGER.Error("Failed to compile shader file");
    glDeleteShader(name_);
    name_ = 0;
    return -1;
  }
  return 0;
}

ShaderProgram::ShaderProgram()
    : shaders_(),
      name_(0) {
}

ShaderProgram::~ShaderProgram() {
  Clean();
}

void ShaderProgram::PushShader(const Shader &shader) {
  shaders_.push_back(shader.name());
}

int ShaderProgram::Link() {
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

void ShaderProgram::Clean() {
  if (name_ != 0) {
    glDeleteProgram(name_);
    name_ = 0;
  }
  shaders_.clear();
}

