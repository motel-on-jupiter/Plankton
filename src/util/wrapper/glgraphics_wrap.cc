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
    assert_equal(strerror_s(errbuf, ARRAYSIZE(errbuf), errno), 0);
    LOGGER.Error("Failed to seek the end of file (errmsg: %s)", errbuf);
  }
  long filesize = ftell(fp);
  if (fseek(fp, 0L, SEEK_SET) != 0) {
    char errbuf[256];
    assert_equal(strerror_s(errbuf, ARRAYSIZE(errbuf), errno), 0);
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

inline void glActiveTextureUnit(GLuint unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
}

inline void glBindDrawFramebuffer(GLuint framebuffer) {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
}

inline void glBindReadFramebuffer(GLuint framebuffer) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
}

inline void glBindRenderbuffer_(GLuint renderbuffer) {
  glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
}

inline void glBindTexture1D(GLuint texture) {
  glBindTexture(GL_TEXTURE_1D, texture);
}

inline void glBindTexture2D(GLuint texture) {
  glBindTexture(GL_TEXTURE_2D, texture);
}

inline void glBindTexture3D(GLuint texture) {
  glBindTexture(GL_TEXTURE_3D, texture);
}

inline GLenum glCheckDrawFramebufferStatus() {
  return glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
}

inline GLenum glCheckReadFramebufferStatus() {
  return glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
}

inline void glClearAll() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

inline void glDrawFramebufferColorTexture(GLint attachidx, GLuint texture,
                                          GLint level) {
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachidx, texture, level);
}

inline void glDrawFramebufferDepthTexture(GLuint texture, GLint level) {
  glFramebufferDepthTexture(GL_DRAW_FRAMEBUFFER, texture, level);
}

inline void glDrawFramebufferRenderbuffer(GLenum attachment,
                                          GLuint renderbuffer) {
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            renderbuffer);
}

inline void glFramebufferColorTexture(GLenum target, GLint attachidx,
                                      GLuint texture, GLint level) {
  ASSERT(attachidx < GL_MAX_COLOR_ATTACHMENTS);
  glFramebufferTexture(target, GL_COLOR_ATTACHMENT0 + attachidx, texture,
                       level);
}

inline void glFramebufferDepthTexture(GLenum target, GLuint texture,
                                      GLint level) {
  glFramebufferTexture(target, GL_DEPTH_ATTACHMENT, texture, level);
}

inline void glFramebufferDrawColorAttachment(GLint attachidx) {
  ASSERT(attachidx < GL_MAX_COLOR_ATTACHMENTS);
  glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachidx);
}

inline void glGenFramebuffer(GLuint *id) {
  glGenFramebuffers(1, id);
}

inline void glGenRenderbuffer(GLuint *renderbuffer) {
  glGenRenderbuffers(1, renderbuffer);
}

inline void glGenTexture(GLuint *texture) {
  glGenTextures(1, texture);
}

inline void glReadFramebufferColorTexture(GLint attachidx, GLuint texture,
                                          GLint level) {
  glFramebufferTexture(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachidx, texture, level);
}

inline void glReadFramebufferDepthTexture(GLuint texture, GLint level) {
  glFramebufferDepthTexture(GL_READ_FRAMEBUFFER, texture, level);
}

inline void glReadFramebufferRenderbuffer(GLenum attachment,
                                          GLuint renderbuffer) {
  glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                            renderbuffer);
}

inline void glUnbindDrawFramebuffer() {
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

inline void glUnbindFramebuffer(GLenum target) {
  glBindFramebuffer(target, 0);
}

inline void glUnbindReadFramebuffer() {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

inline void glSetClearingColor(GLclampf red, GLclampf green, GLclampf blue,
                               GLclampf alpha) {
  glClearColor(red, green, blue, alpha);
}
