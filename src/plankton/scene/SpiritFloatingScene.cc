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

const char *SpiritFloatingSceneRenderer::kShaderPaths[] = {
    "share/shader/v_pipeline.glsl", "share/shader/f_pipeline.glsl",
    "share/shader/v_blend.glsl", "share/shader/f_blend.glsl",
    "share/shader/v_filter.glsl", "share/shader/f_none_filter.glsl", };

SpiritFloatingSceneRenderer::SpiritFloatingSceneRenderer()
    : initialized_(false),
      shaders_(),
      shaderps_(),
      framebuf_() {
}

SpiritFloatingSceneRenderer::~SpiritFloatingSceneRenderer() {

}

int SpiritFloatingSceneRenderer::Initialize(const glm::vec2 &window_size) {
  if (initialized_) {
    LOGGER.Notice("Already initialized");
    return 1;
  }

  if (framebuf_.SetUp(window_size, 2) < 0) {
    LOGGER.Error("Faield to set up the frame buffer");
    return -1;
  }
  for (auto it = framebuf_.colortexs().begin();
      it != framebuf_.colortexs().end(); ++it) {
    glBindTexture(GL_TEXTURE_2D, *it);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }

  for (int i = 0; i < ARRAYSIZE(kShaderPaths) / 2; ++i) {
    GLShader *vshader = new GLShader(GL_VERTEX_SHADER, kShaderPaths[i * 2]);
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

    GLShader *fshader = new GLShader(GL_FRAGMENT_SHADER, kShaderPaths[i * 2 + 1]);
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

    GLShaderProgram *program = new GLShaderProgram();
    if (program == nullptr) {
      LOGGER.Error("Failed to create shader program object");
      return -1;
    }
    program->PushShader(*vshader);
    program->PushShader(*fshader);
    if (program->Link() < 0) {
      LOGGER.Error("Failed to link program");
      delete program;
      Finalize();
      return -1;
    }
    shaderps_.push_back(program);
  }
  initialized_ = true;
  return 0;
}

void SpiritFloatingSceneRenderer::Finalize() {
  initialized_ = false;

  for (auto it = shaderps_.begin(); it != shaderps_.end(); ++it) {
    delete *it;
  }
  shaderps_.clear();
  for (auto it = shaders_.begin(); it != shaders_.end(); ++it) {
    delete *it;
  }
  shaders_.clear();

  framebuf_.CleanUp();
}

void SpiritFloatingSceneRenderer::Begin(const glm::vec2 &window_size) {
  // Set up view-port
  glViewport(0, 0, static_cast<GLsizei>(window_size.x),
             static_cast<GLsizei>(window_size.y));
  glEnable(GL_DEPTH_TEST);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
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

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf_.name());
  GLenum attachments[] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, attachments);
  glUseProgram(shaderps_.at(0)->name());

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(kViewMatrix));
}

void SpiritFloatingSceneRenderer::End() {
  glPopMatrix();

  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuf_.name());
  GLenum attachment1[] = { GL_COLOR_ATTACHMENT1 };
  glDrawBuffers(1, attachment1);

  glUseProgram(shaderps_.at(1)->name());
  glUniform1i(glGetUniformLocation(shaderps_.at(1)->name(), "texture0"), 0);
  glUniform1i(glGetUniformLocation(shaderps_.at(1)->name(), "texture1"), 1);
  glUniform1f(glGetUniformLocation(shaderps_.at(1)->name(), "blend0"), 0.3f);
  glUniform1f(glGetUniformLocation(shaderps_.at(1)->name(), "blend1"), 0.8f);
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, framebuf_.colortexs().at(0));
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, framebuf_.colortexs().at(1));
  glActiveTexture(GL_TEXTURE0);

  static const GLfloat quad_vertices[4][3] = { { -1.0f, -1.0f, 0.0f }, { -1.0f,
      1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, };
  static const GLfloat tex_coords[4][3] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, {
      1.0f, 1.0f }, { 1.0f, 0.0f }, };
  glBegin(GL_QUADS);
  for (int i = 0; i < 4; ++i) {
    glTexCoord2fv(tex_coords[i]);
    glVertex3fv(quad_vertices[i]);
  }
  glEnd();

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glUseProgram(shaderps_.at(2)->name());
  glBindTexture(GL_TEXTURE_2D, framebuf_.colortexs().at(1));

  glBegin(GL_QUADS);
  for (int i = 0; i < 4; ++i) {
    glTexCoord2fv(tex_coords[i]);
    glVertex3fv(quad_vertices[i]);
  }
  glEnd();

  glUseProgram(0);
  glDisable(GL_TEXTURE_2D);
}

SpiritFloatingScene::SpiritFloatingScene()
    : initialize_(false),
      spirits_(),
      renderer_() {
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
  if (!initialize_) {
    return;
  }

  renderer_.Begin(window_size);
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
