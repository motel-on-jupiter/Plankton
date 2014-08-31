/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "GLFrameBuffer.h"
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"

GLFrameBuffer::GLFrameBuffer()
    : name_(0),
      renderbuf_(0),
      colortexs_(),
      depthtex_(0) {
}

GLFrameBuffer::~GLFrameBuffer() {
}

int GLFrameBuffer::SetUp(const glm::vec2 &window_size, int num_color_tex) {
  GLsizei width = static_cast<GLsizei>(window_size.x);
  GLsizei height = static_cast<GLsizei>(window_size.y);

  glGenFramebuffer(&name_);
  glBindDrawFramebuffer(name_);

  glGenRenderbuffer(&renderbuf_);
  glBindRenderbuffer_(renderbuf_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
  glDrawFramebufferRenderbuffer(GL_DEPTH_ATTACHMENT, renderbuf_);

  for (int i=0; i<num_color_tex; ++i) {
    GLuint colortex = 0;
    glGenTexture(&colortex);
    glBindTexture2D(colortex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glDrawFramebufferColorTexture(i, colortex, 0);
    colortexs_.push_back(colortex);
  }

  glGenTexture(&depthtex_);
  glBindTexture2D(depthtex_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glDrawFramebufferDepthTexture(depthtex_, 0);

  GLenum status = glCheckDrawFramebufferStatus();
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    LOGGER.Error("Failed to complete frame buffer (status: %d)", status);
    return -1;
  }
  return 0;
}

void GLFrameBuffer::CleanUp() {
  if (depthtex_ != 0) {
    glDeleteTextures(1, &depthtex_);
    depthtex_ = 0;
  }
  for (auto it = colortexs_.begin(); it != colortexs_.end(); ++it) {
    glDeleteTextures(1, &(*it));
  }
  colortexs_.clear();
  if (renderbuf_ != 0) {
    glDeleteRenderbuffers(1, &renderbuf_);
    renderbuf_ = 0;
  }
  if (name_ != 0) {
    glDeleteFramebuffers(1, &name_);
    name_ = 0;
  }
}
