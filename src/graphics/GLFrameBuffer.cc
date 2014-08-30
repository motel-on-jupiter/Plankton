/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "GLFrameBuffer.h"
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"

GLFrameBuffer::GLFrameBuffer()
    : name_(0),
      renderbuf_(0),
      colortex_(0),
      depthtex_(0) {
}

GLFrameBuffer::~GLFrameBuffer() {
}

int GLFrameBuffer::SetUp(const glm::vec2 &window_size) {
  glGenFramebuffers(1, &name_);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, name_);

  glGenRenderbuffers(1, &renderbuf_);
  glBindRenderbuffer(GL_RENDERBUFFER, renderbuf_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                        static_cast<GLsizei>(window_size.x),
                        static_cast<GLsizei>(window_size.y));
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, renderbuf_);

  glGenTextures(1, &colortex_);
  glBindTexture(GL_TEXTURE_2D, colortex_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(window_size.x),
               static_cast<GLsizei>(window_size.y), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colortex_, 0);
  GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glGenTextures(1, &depthtex_);
  glBindTexture(GL_TEXTURE_2D, depthtex_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
               static_cast<GLsizei>(window_size.x),
               static_cast<GLsizei>(window_size.y), 0, GL_DEPTH_COMPONENT,
               GL_FLOAT, nullptr);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthtex_, 0);

  if (glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LOGGER.Error("Failed to complete frame buffer");
    return -1;
  }
  return 0;
}

void GLFrameBuffer::CleanUp() {
  if (depthtex_ != 0) {
    glDeleteTextures(1, &depthtex_);
    depthtex_ = 0;
  }
  if (colortex_ != 0) {
    glDeleteTextures(1, &colortex_);
    colortex_ = 0;
  }
  if (renderbuf_ != 0) {
    glDeleteRenderbuffers(1, &renderbuf_);
    renderbuf_ = 0;
  }
  if (name_ != 0) {
    glDeleteFramebuffers(1, &name_);
    name_ = 0;
  }
}
