/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef GL_FRAME_BUFFER_H_
#define GL_FRAME_BUFFER_H_

class GLFrameBuffer {
 public:
  GLFrameBuffer();
  virtual ~GLFrameBuffer();

  int SetUp(const glm::vec2 &window_size);
  void CleanUp();

  GLuint name() const {
    return name_;
  }
  GLuint renderbuf() const {
    return renderbuf_;
  }
  GLuint colortex() const {
    return colortex_;
  }
  GLuint depthtex() const {
    return depthtex_;
  }

 private:
  GLuint name_;
  GLuint renderbuf_;
  GLuint colortex_;
  GLuint depthtex_;
};

#endif /* GL_FRAME_BUFFER_H_ */
