/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "plankton/scene/SpiritFloatingScene.h"
#include <algorithm>
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"
#include "util/macro_util.h"
#include "util/math_aux.h"

int Spirit::Initialize() {
  set_pos(glm::vec3(0.0f, 5.0f, 0.0f));

  quadric_ = gluNewQuadric();
  if (quadric_ == nullptr) {
    LOGGER.Error("Failed to create quadric object");
    return -1;
  }
  return 0;
}

void Spirit::Finalize() {
  gluDeleteQuadric(quadric_);
  quadric_ = nullptr;
}

void Spirit::Update(float elapsed_time) {
  if (is_fzero(glm::distance(pos(), goal_))) {
    goal_ = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
  }
  float dist = std::min<float>(glm::distance(pos(), goal_), speed_ * elapsed_time);
  set_pos(pos() + glm::normalize(goal_ - pos()) * dist);
}

void Spirit::Draw() {
  glMultMatrixf(glm::value_ptr(glm::translate(pos())));
  gluSphere(quadric_, 1.0, 30, 30);
}

SpiritFloatingScene::SpiritFloatingScene()
: initialize_(false), spirit_() {
}

int SpiritFloatingScene::Initialize() {
  if (initialize_) {
    return 1;
  }

  // Initialize spirit object
  int ret = spirit_.Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the spirit object (ret: %d)", ret);
    return -1;
  }

  // Set up lighting
  static GLfloat position[] = {0.0f, 10.0f, -10.0f, 1.0f};
  static GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  initialize_ = true;
  return 0;
}

void SpiritFloatingScene::Finalize() {
  if (!initialize_) {
    return;
  }
  initialize_ = false;

  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  spirit_.Finalize();
}

void SpiritFloatingScene::Update(float elapsed_time) {
  if (!initialize_) {
    return;
  }
  spirit_.Update(elapsed_time);
  return;
}

void SpiritFloatingScene::Draw() {
  if (!initialize_) {
    return;
  }
  // setup material
  GLfloat ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat diffuse[] = {0.1f, 0.1f, 0.1f, 1.0f};
  GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat shininess[] = {1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

  // draw sphere
  spirit_.Draw();
}

int SpiritFloatingScene::OnMouseButtonDown(unsigned char button,
                                           const glm::vec2& cursor_pos) {
  UNUSED(button);
  UNUSED(cursor_pos);
  return 0;
}
