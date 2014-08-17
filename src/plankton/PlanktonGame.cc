/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "PlanktonGame.h"

#include <GL/glew.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <SDL_video.h>

#include "plankton/scene/SpiritFloatingScene.h"
#include "util/logging/Logger.h"
#include "util/macro_util.h"

PlanktonGame::PlanktonGame() : scene_(nullptr), font_(nullptr) {
}

PlanktonGame::~PlanktonGame() {
  Finalize();
}

int PlanktonGame::Initialize(const glm::vec2 &window_size) {
  UNUSED(window_size);

  // Load font
  font_ = TTF_OpenFont("share/ipag00303/ipag.ttf", 24);
  if (font_ == nullptr) {
    LOGGER.Error("Failed to open font with SDL_ttf (errmsg: %s)",
                 TTF_GetError());
    return -1;
  }

  // Set up view-port
  glViewport(0, 0, static_cast<int>(window_size.x), static_cast<int>(window_size.y));
  glEnable(GL_DEPTH_TEST);

  // Set up projection matrix
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,
                 static_cast<GLdouble>(window_size.x) / static_cast<GLdouble>(window_size.y),
                 2.0, 200.0);

  return 0;
}

void PlanktonGame::Finalize() {
  if (scene_ != nullptr) {
    scene_->Finalize();
    delete scene_;
    scene_ = nullptr;
  }
  if (font_ != nullptr) {
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
}

void PlanktonGame::Update(float elapsed_time) {
  if (scene_ == nullptr) {
    return;
  }
  scene_->Update(elapsed_time);
}

void PlanktonGame::Draw(const glm::vec2 &window_size) {
  UNUSED(window_size);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (scene_ == nullptr) {
    return;
  }

  // setup view
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0f, 0.0f, -30.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  scene_->Draw();
  glPopMatrix();
}

int PlanktonGame::OnKeyboardDown(SDL_Keycode key) {
  if (scene_ == nullptr) {
    if (key == SDLK_1) {
      LOGGER.Info("Set up the Sprit-Floating scene");
      scene_ = new SpiritFloatingScene();
      if (scene_ == nullptr) {
        LOGGER.Error("Failed to create the scene object");
        return -1;
      }
      int ret = scene_->Initialize();
      if (ret < 0) {
        LOGGER.Error("Failed to setup the scene (ret: %d)", ret);
        return -1;
      }
    }
  } else {
    if (key == SDLK_0) {
      LOGGER.Info("Clean up the current scene");
      scene_->Finalize();
      delete scene_;
      scene_ = nullptr;
    }
  }
  return 0;
}

int PlanktonGame::OnMouseButtonDown(unsigned char button, int x, int y, const glm::vec2 &window_size) {
  if (scene_ != nullptr) {
    auto abs_cursor_pos = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    return scene_->OnMouseButtonDown(button, abs_cursor_pos / window_size);
  }
  return 0;
}
