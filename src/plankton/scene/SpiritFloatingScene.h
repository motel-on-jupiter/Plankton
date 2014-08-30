/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SPIRITFLOATINGSCENE_H_
#define SPIRITFLOATINGSCENE_H_

#include <vector>
#include "entity/BaseEntity.h"
#include "plankton/PlanktonGame.h"

class BaseSpirit : public BaseEntity {
 public:
  BaseSpirit(const glm::vec3 &color) : BaseEntity(), quadric_(nullptr), color_(color) {}
  virtual ~BaseSpirit() {}

  virtual int Initialize();
  virtual void Finalize();
  virtual void Update(float elapsed_time) = 0;
  virtual void Draw();

 private:
  static const GLfloat kSpecularColor[];
  static const GLfloat kShininess;

  GLUquadric *quadric_;
  glm::vec3 color_;
};

class RandomSpirit : public BaseSpirit {
 public:
  RandomSpirit(const glm::vec3 &color, float speed)
 : BaseSpirit(color), goal_(), speed_(speed) {}
  virtual ~RandomSpirit() {}

  virtual void Update(float elapsed_time);

 private:
  glm::vec3 goal_;
  float speed_;
};

class CatmullRomSpirit : public BaseSpirit {
 public:
  CatmullRomSpirit(const glm::vec3 &color, float step);
  virtual ~CatmullRomSpirit() {}

  virtual void Update(float elapsed_time);

 private:
  glm::vec3 targets_[4];
  float time_;
  float step_;
};

class HermiteSpirit : public BaseSpirit {
 public:
  HermiteSpirit(const glm::vec3 &color, float step);
  virtual ~HermiteSpirit() {}

  virtual void Update(float elapsed_time);

 private:
  glm::vec3 vs_[2];
  glm::vec3 ts_[2];
  float time_;
  float step_;
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
  std::vector<BaseSpirit *> spirits_;
};
#endif /* SPIRITSCENE_H_ */
