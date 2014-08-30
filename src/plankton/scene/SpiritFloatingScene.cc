/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "SpiritFloatingScene.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include "plankton/actor/Spirit.h"
#include "util/catalogue/color_sample.h"
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"
#include "util/macro_util.h"
#include "util/math_aux.h"

const float SpiritFloatingScene::kPerspectiveFovy = glm::radians(45.0f);
const float SpiritFloatingScene::kPerspectiveNear = 2.0f;
const float SpiritFloatingScene::kPerspectiveFar = 200.0f;

const GLfloat SpiritFloatingScene::kLightPosition[] = { 0.0f, 10.0f, -10.0f,
    1.0f };
const GLfloat SpiritFloatingScene::kLightAmbientColor[] = { 1.0f, 1.0f, 1.0f,
    1.0f };
const GLfloat SpiritFloatingScene::kLightDiffuseColor[] = { 1.0f, 1.0f, 1.0f,
    1.0f };
const GLfloat SpiritFloatingScene::kLightSpecularColor[] = { 0.1f, 0.1f, 0.1f,
    1.0f };

const glm::mat4 SpiritFloatingScene::kViewMatrix = glm::lookAt(
    glm::vec3(0.0f, 0.0f, -30.0f), glm::vec3(0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

SpiritFloatingScene::SpiritFloatingScene()
    : initialize_(false),
      spirits_() {
}

int SpiritFloatingScene::Initialize(const glm::vec2 &window_size) {
  if (initialize_) {
    return 1;
  }

  // Initialize spirit object
  BaseSpirit *spirit = new RandomSpirit(glm::vec3(0.0f, 5.0f, 0.0f),
                                        X11Color::to_fvec(X11Color::kGray),
                                        1.0f);
  if (spirit == nullptr) {
    LOGGER.Error("Failed to create the random spirit object");
    return -1;
  }
  int ret = spirit->Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the random spirit object (ret: %d)",
                 ret);
    return -1;
  }
  spirits_.push_back(spirit);
  spirit = new CatmullRomSpirit(glm::vec3(0.0f, 5.0f, 0.0f),
                                X11Color::to_fvec(X11Color::kGreen), 1.0f);
  if (spirit == nullptr) {
    LOGGER.Error("Failed to create the catmull row spirit object");
    return -1;
  }
  ret = spirit->Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the catmull row spirit object (ret: %d)",
                 ret);
    return -1;
  }
  spirits_.push_back(spirit);
  spirit = new HermiteSpirit(glm::vec3(0.0f, 5.0f, 0.0f),
                             X11Color::to_fvec(X11Color::kOrange), 1.0f);
  if (spirit == nullptr) {
    LOGGER.Error("Failed to create the hermite spirit object");
    return -1;
  }
  ret = spirit->Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the hermite spirit object (ret: %d)",
                 ret);
    return -1;
  }
  spirits_.push_back(spirit);

  // Set up view-port
  glViewport(0, 0, static_cast<GLsizei>(window_size.x),
             static_cast<GLsizei>(window_size.y));
  glEnable(GL_DEPTH_TEST);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glLoadMatrixf(
      glm::value_ptr(
          glm::perspective(kPerspectiveFovy, window_size.x / window_size.y,
                           kPerspectiveNear, kPerspectiveFar)));

  // Set up lighting
  glLightfv(GL_LIGHT0, GL_POSITION, kLightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, kLightAmbientColor);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, kLightDiffuseColor);
  glLightfv(GL_LIGHT0, GL_SPECULAR, kLightSpecularColor);
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

  BOOST_FOREACH(BaseSpirit *spirit, spirits_) {
    spirit->Finalize();
    delete spirit;
  }
  spirits_.clear();
}

void SpiritFloatingScene::Update(float elapsed_time,
                                 const glm::vec2 &window_size) {
  UNUSED(window_size);

  if (!initialize_) {
    return;
  }

  BOOST_FOREACH(BaseSpirit *spirit, spirits_) {
    spirit->Update(elapsed_time);
  }
  return;
}

void SpiritFloatingScene::Draw(const glm::vec2 &window_size) {
  UNUSED(window_size);

  if (!initialize_) {
    return;
  }

  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(kViewMatrix));
  BOOST_FOREACH(BaseSpirit *spirit, spirits_) {
    spirit->Draw();
  }
  glPopMatrix();
}

int SpiritFloatingScene::OnMouseButtonDown(unsigned char button,
                                           const glm::vec2& cursor_pos) {
  UNUSED(button);
  UNUSED(cursor_pos);

  return 0;
}
