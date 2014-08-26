/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SPIRITFLOATINGSCENE_H_
#define SPIRITFLOATINGSCENE_H_

#include "entity/BaseEntity.h"
#include "plankton/PlanktonGame.h"

class Spirit : public BaseEntity {
 public:
  Spirit() : BaseEntity(), quadric_(nullptr), speed_(50.0f), goal_() {}
  ~Spirit() {}

  virtual int Initialize();
  virtual void Finalize();
  virtual void Update(float elapsed_time);
  virtual void Draw();

 private:
  static const GLfloat kAmbientColor[];
  static const GLfloat kDiffuseColor[];
  static const GLfloat kSpecularColor[];
  static const GLfloat kShininess;

  GLUquadric *quadric_;
  float speed_;
  glm::vec3 goal_;
};

class SpiritFloatingScene : public PlanktonGameSceneInterface {
 public:
  SpiritFloatingScene();
  virtual ~SpiritFloatingScene() {}

  virtual int Initialize(const glm::vec2 &window_size);
  virtual void Finalize();
  virtual void Update(float elapsed_time, const glm::vec2 &window_size);
  virtual void Draw(const glm::vec2 &window_size);
  virtual int OnMouseButtonDown(unsigned char button, const glm::vec2 &cursor_pos);

 private:
  static const float kPerspectiveFovy;
  static const float kPerspectiveNear;
  static const float kPerspectiveFar;
  static const GLfloat kLightPosition[];
  static const GLfloat kLightAmbientColor[];
  static const GLfloat kLightDiffuseColor[];
  static const GLfloat kLightSpecularColor[];
  static const glm::mat4 kViewMatrix;

  bool initialize_;
  Spirit spirit_;
};
#endif /* SPIRITSCENE_H_ */
