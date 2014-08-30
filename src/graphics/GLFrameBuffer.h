/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef GL_FRAME_BUFFER_H_
#define GL_FRAME_BUFFER_H_

#include <vector>

class GLFrameBuffer {
 public:
  GLFrameBuffer();
  virtual ~GLFrameBuffer();

  int SetUp(const glm::vec2 &window_size, int num_color_tex);
  void CleanUp();

  GLuint name() const {
    return name_;
  }
  GLuint renderbuf() const {
    return renderbuf_;
  }
  const std::vector<GLuint> &colortexs() const {
    return colortexs_;
  }
  GLuint depthtex() const {
    return depthtex_;
  }

 private:
  GLuint name_;
  GLuint renderbuf_;
  std::vector<GLuint> colortexs_;
  GLuint depthtex_;
};

#endif /* GL_FRAME_BUFFER_H_ */
