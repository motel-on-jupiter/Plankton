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

const float SpiritFloatingSceneRenderer::kPerspectiveFovy = glm::radians(45.0f);
const float SpiritFloatingSceneRenderer::kPerspectiveNear = 2.0f;
const float SpiritFloatingSceneRenderer::kPerspectiveFar = 200.0f;

const GLfloat SpiritFloatingSceneRenderer::kLightPosition[] = { 0.0f, 10.0f,
    -10.0f, 1.0f };
const GLfloat SpiritFloatingSceneRenderer::kLightAmbientColor[] = { 1.0f, 1.0f,
    1.0f, 1.0f };
const GLfloat SpiritFloatingSceneRenderer::kLightDiffuseColor[] = { 1.0f, 1.0f,
    1.0f, 1.0f };
const GLfloat SpiritFloatingSceneRenderer::kLightSpecularColor[] = { 0.1f, 0.1f,
    0.1f, 1.0f };

const glm::mat4 SpiritFloatingSceneRenderer::kViewMatrix = glm::lookAt(
    glm::vec3(0.0f, 0.0f, -30.0f), glm::vec3(0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

SpiritFloatingSceneRenderer::SpiritFloatingSceneRenderer()
    : initialized_(false),
      shaders_(),
      shader_program_() {
}

SpiritFloatingSceneRenderer::~SpiritFloatingSceneRenderer() {

}

int SpiritFloatingSceneRenderer::Initialize(const glm::vec2 &window_size) {
  if (initialized_) {
    LOGGER.Notice("Already initialized");
    return 1;
  }

  Shader *vshader = new Shader(GL_VERTEX_SHADER,
                               "share/shader/v_pipeline.glsl");
  if (vshader == nullptr) {
    LOGGER.Error("Failed to create vertex shader object");
    return -1;
  }
  if (vshader->Compile() < 0) {
    LOGGER.Error("Failed to compile vertex shader");
    delete vshader;
    return -1;
  }
  shaders_.push_back(vshader);

  Shader *fshader = new Shader(GL_FRAGMENT_SHADER,
                               "share/shader/f_pipeline.glsl");
  if (fshader == nullptr) {
    LOGGER.Error("Failed to create fragment shader object");
    return -1;
  }
  if (fshader->Compile() < 0) {
    LOGGER.Error("Failed to compile fragment shader");
    delete fshader;
    Finalize();
    return -1;
  }
  shaders_.push_back(fshader);

  shader_program_.PushShader(*vshader);
  shader_program_.PushShader(*fshader);
  if (shader_program_.Link() < 0) {
    LOGGER.Error("Failed to link program");
    Finalize();
    return -1;
  }

  glUseProgram(shader_program_.name());

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

  return 0;
}

void SpiritFloatingSceneRenderer::Finalize() {
  initialized_ = false;

  shader_program_.Clean();
  for (auto it = shaders_.begin(); it != shaders_.end(); ++it) {
    delete *it;
  }
  shaders_.clear();

  glUseProgram(0);
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
}

void SpiritFloatingSceneRenderer::Begin() {
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(kViewMatrix));
}

void SpiritFloatingSceneRenderer::End() {
  glPopMatrix();
}

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
    delete spirit;
    return -1;
  }
  spirits_.push_back(spirit);
  spirit = new CatmullRomSpirit(glm::vec3(0.0f, 5.0f, 0.0f),
                                X11Color::to_fvec(X11Color::kGreen), 1.0f);
  if (spirit == nullptr) {
    LOGGER.Error("Failed to create the catmull row spirit object");
    Finalize();
    return -1;
  }
  ret = spirit->Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the catmull row spirit object (ret: %d)",
                 ret);
    delete spirit;
    Finalize();
    return -1;
  }
  spirits_.push_back(spirit);
  spirit = new HermiteSpirit(glm::vec3(0.0f, 5.0f, 0.0f),
                             X11Color::to_fvec(X11Color::kOrange), 1.0f);
  if (spirit == nullptr) {
    LOGGER.Error("Failed to create the hermite spirit object");
    Finalize();
    return -1;
  }
  ret = spirit->Initialize();
  if (ret < 0) {
    LOGGER.Error("Failed to initialize the hermite spirit object (ret: %d)",
                 ret);
    delete spirit;
    Finalize();
    return -1;
  }
  spirits_.push_back(spirit);

  if (renderer_.Initialize(window_size) < 0) {
    LOGGER.Error("Failed to initialize the renderer");
    Finalize();
    return -1;
  }

  initialize_ = true;
  return 0;
}

void SpiritFloatingScene::Finalize() {
  initialize_ = false;

  renderer_.Finalize();

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

  renderer_.Begin();
  BOOST_FOREACH(BaseSpirit *spirit, spirits_) {
    spirit->Draw();
  }
  renderer_.End();
}

int SpiritFloatingScene::OnMouseButtonDown(unsigned char button,
                                           const glm::vec2& cursor_pos) {
  UNUSED(button);
  UNUSED(cursor_pos);

  return 0;
}
