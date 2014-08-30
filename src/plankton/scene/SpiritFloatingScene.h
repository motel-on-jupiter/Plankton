/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SPIRITFLOATINGSCENE_H_
#define SPIRITFLOATINGSCENE_H_

#include <vector>
#include "graphics/GLFrameBuffer.h"
#include "graphics/GLShader.h"
#include "plankton/actor/Spirit.h"
#include "plankton/PlanktonGame.h"

class SpiritFloatingSceneRenderer : public PlanktonGameSceneRendererInterface {
 public:
  SpiritFloatingSceneRenderer();
  virtual ~SpiritFloatingSceneRenderer();

  virtual int Initialize(const glm::vec2 &window_size);
  virtual void Finalize();

  virtual void Begin(const glm::vec2 &window_size);
  virtual void End();

 private:
  static const float kPerspectiveFovy;
  static const float kPerspectiveNear;
  static const float kPerspectiveFar;
  static const GLfloat kLightPosition[];
  static const GLfloat kLightAmbientColor[];
  static const GLfloat kLightDiffuseColor[];
  static const GLfloat kLightSpecularColor[];
  static const glm::mat4 kViewMatrix;
  static const char *kShaderPaths[];

  bool initialized_;
  std::vector<GLShader *> shaders_;
  std::vector<GLShaderProgram *> shaderps_;
  GLFrameBuffer framebuf_;
};

class SpiritFloatingScene : public PlanktonGameSceneInterface {
 public:
  SpiritFloatingScene();
  virtual ~SpiritFloatingScene() {
  }

  virtual int Initialize(const glm::vec2 &window_size);
  virtual void Finalize();
  virtual void Update(float elapsed_time, const glm::vec2 &window_size);
  virtual void Draw(const glm::vec2 &window_size);
  virtual int OnMouseButtonDown(unsigned char button,
                                const glm::vec2 &cursor_pos);

 private:
  bool initialize_;
  std::vector<BaseSpirit *> spirits_;
  SpiritFloatingSceneRenderer renderer_;
};
#endif /* SPIRITFLOATINGSCENE_H_ */
