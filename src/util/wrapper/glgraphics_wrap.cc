/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "glgraphics_wrap.h"
#include <stddef.h>
#include "util/logging/Logger.h"
#include "util/assert_util.h"
#include "util/macro_util.h"

int glCompileShaderFile(GLuint shader, const char *path) {
  // Buffer the shader source
  FILE *fp = nullptr;
  if (fopen_s(&fp, path, "rb") != 0) {
    LOGGER.Error("Failed to open shader file");
    return -1;
  }
  if (fseek(fp, 0L, SEEK_END) != 0) {
    char errbuf[256];
    ASSERT(strerror_s(errbuf, ARRAYSIZE(errbuf), errno) == 0);
    LOGGER.Error("Failed to seek the end of file (errmsg: %s)", errbuf);
  }
  long filesize = ftell(fp);
  if (fseek(fp, 0L, SEEK_SET) != 0) {
    char errbuf[256];
    ASSERT(strerror_s(errbuf, ARRAYSIZE(errbuf), errno) == 0);
    LOGGER.Error("Failed to seek the start of file (errmsg: %s)", errbuf);
  }
  GLchar *source = new char[filesize / sizeof(char) + 1];
  if (source == nullptr) {
    LOGGER.Error("Failed to allocate source buffer");
    return -1;
  }
  do {
    fread(source, 1, filesize, fp);
  } while (errno == EINVAL);
  fclose(fp);
  source[filesize] = '\0';

  // Load the shader source
  const GLchar *csource = source;
  glShaderSource(shader, 1, &csource, nullptr);
  delete source;

  // Compile the shader
  glCompileShader(shader);
  GLint compileok = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileok);
  if (compileok == GL_FALSE) {
    GLint loglen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
    char *log = new char[loglen];
    if (log == nullptr) {
      LOGGER.Error("Failed to allocate log buffer");
      return -1;
    }
    glGetShaderInfoLog(shader, loglen, nullptr, log);
    LOGGER.Error("Failed to compile shader (errmsg: %s)", log);
    delete log;
    return -1;
  }

  return 0;
}

int glLinkProgramWithShaders(GLuint program, std::vector<GLuint> &shaders) {
  // Attach to program
  for (auto it = shaders.begin(); it != shaders.end(); ++it) {
    glAttachShader(program, *it);
  }

  // Link the program
  glLinkProgram(program);
  GLint linkok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &linkok);
  if (linkok == GL_FALSE) {
    GLint loglen = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglen);
    char *log = new char[loglen];
    glGetProgramInfoLog(program, loglen, nullptr, log);
    LOGGER.Error("Failed to link program (errmsg: %s)", log);
    free(log);
    return -1;
  }
  return 0;
}
