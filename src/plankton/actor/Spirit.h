/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SPIRIT_H_
#define SPIRIT_H_

#include "entity/BaseEntity.h"
#include "entity/EntityDraw.h"

class BaseSpirit : public BaseEntity, public SphereEntityDraw {
 public:
  BaseSpirit(const glm::vec3& pos, const glm::vec3 &color);
  virtual ~BaseSpirit() {
  }

  virtual int Initialize();
  virtual void Finalize();
  virtual void Update(float elapsed_time) = 0;

 private:
  GLUquadric *quadric_;
};

class RandomSpirit : public BaseSpirit {
 public:
  RandomSpirit(const glm::vec3& pos, const glm::vec3 &color, float step);
  virtual ~RandomSpirit() {
  }

  virtual void Update(float elapsed_time);

 private:
  float step_;
  glm::vec3 start_;
  glm::vec3 goal_;
  float time_;
};

class CatmullRomSpirit : public BaseSpirit {
 public:
  CatmullRomSpirit(const glm::vec3& pos, const glm::vec3 &color, float step);
  virtual ~CatmullRomSpirit() {
  }

  virtual void Update(float elapsed_time);

 private:
  float step_;
  glm::vec3 targets_[4];
  float time_;
};

class HermiteSpirit : public BaseSpirit {
 public:
  HermiteSpirit(const glm::vec3& pos, const glm::vec3 &color, float step);
  virtual ~HermiteSpirit() {
  }

  virtual void Update(float elapsed_time);

 private:
  float step_;
  glm::vec3 vs_[2];
  glm::vec3 ts_[2];
  float time_;
};

#endif /* SPIRIT_H_ */
