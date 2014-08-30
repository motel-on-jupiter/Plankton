/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "Spirit.h"
#include "util/catalogue/color_sample.h"
#
BaseSpirit::BaseSpirit(const glm::vec3& pos, const glm::vec3 &color)
    : BaseEntity(pos, 0.0f, glm::vec3(2.0f)),
      SphereEntityDraw(*this, color, X11Color::to_fvec(X11Color::kWhite), 1.0f,
                       30, 30),
      quadric_(nullptr) {
}

int BaseSpirit::Initialize() {
  return SphereEntityDraw::Initialize();
}

void BaseSpirit::Finalize() {
  SphereEntityDraw::Finalize();
}

RandomSpirit::RandomSpirit(const glm::vec3& pos, const glm::vec3 &color,
                           float step)
    : BaseSpirit(pos, color),
      step_(step),
      start_(pos),
      goal_(),
      time_(0.0f) {
}

void RandomSpirit::Update(float elapsed_time) {
  while (time_ > 1.0f) {
    start_ = goal_;
    goal_ = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
    time_ -= 1.0f;
  }
  set_pos(glm::lerp(start_, goal_, time_));
  time_ += step_ * elapsed_time;
}

HermiteSpirit::HermiteSpirit(const glm::vec3& pos, const glm::vec3 &color,
                             float step)
    : BaseSpirit(pos, color),
      step_(step),
      vs_(),
      ts_(),
      time_(0.0f) {
  for (int i = 0; i < 2; ++i) {
    vs_[i] = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
    ts_[i] = glm::sphericalRand(1.0f);
  }
}

void HermiteSpirit::Update(float elapsed_time) {
  while (time_ > 1.0f) {
    vs_[0] = vs_[1];
    ts_[0] = ts_[1];
    vs_[1] = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
    ts_[1] = glm::sphericalRand(1.0f);
    time_ -= 1.0f;
  }
  set_pos(glm::hermite(vs_[0], ts_[0], vs_[1], ts_[1], time_));
  time_ += step_ * elapsed_time;
}

CatmullRomSpirit::CatmullRomSpirit(const glm::vec3 &pos, const glm::vec3 &color,
                                   float step)
    : BaseSpirit(pos, color),
      step_(step),
      targets_(),
      time_(0.0f) {
  for (int i = 0; i < 4; ++i) {
    targets_[i] = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
  }
}

void CatmullRomSpirit::Update(float elapsed_time) {
  while (time_ > 1.0f) {
    for (int i = 0; i < 3; ++i) {
      targets_[i] = targets_[i + 1];
    }
    targets_[3] = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
    time_ -= 1.0f;
  }
  set_pos(
      glm::catmullRom(targets_[0], targets_[1], targets_[2], targets_[3],
                      time_));
  time_ += step_ * elapsed_time;
}
