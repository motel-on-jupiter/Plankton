/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "SpectrumWaveScene.h"
#include "util/catalogue/color_sample.h"
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"
#include "util/macro_util.h"
#include "util/math_aux.h"

const size_t SpectrumWaveScene::kNumBars = 100;

SpectrumWaveScene::SpectrumWaveScene()
    : initialize_(false),
      remain_time_(FLT_MAX) {
}

int SpectrumWaveScene::Initialize(const glm::vec2 &window_size) {
  if (initialize_) {
    return 1;
  }

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(
      glm::value_ptr(
          glm::ortho(0.0f, window_size.x, window_size.y, 0.0f, -1.0f, 1.0f)));
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  initialize_ = true;
  return 0;
}

void SpectrumWaveScene::Finalize() {
  if (!initialize_) {
    return;
  }
  initialize_ = false;
}

void SpectrumWaveScene::Update(float elapsed_time,
                               const glm::vec2 &window_size) {
  if (!initialize_) {
    return;
  }

  if (remain_time_ > 0.1f) {
    bar_lengths_.clear();
    for (size_t i = 0; i < kNumBars; ++i) {
      bar_lengths_.push_back(glm::linearRand(0.0f, window_size.y));
    }
    remain_time_ = 0.0f;
  }
  remain_time_ += elapsed_time;

  return;
}

void SpectrumWaveScene::Draw(const glm::vec2 &window_size) {
  if (!initialize_) {
    return;
  }

  glColor3ubv(WebColor::kPurple);
  const float bar_width = window_size.x / static_cast<float>(kNumBars);

  glMatrixMode(GL_MODELVIEW);
  float x = 0.0f;
  for (auto it = bar_lengths_.begin(); it != bar_lengths_.end(); ++it) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, window_size.y);
    glVertex2f(x + bar_width, window_size.y);
    glVertex2f(x + bar_width, window_size.y - *it);
    glVertex2f(x, window_size.y - *it);
    glEnd();
    x += bar_width;
  }
}

int SpectrumWaveScene::OnMouseButtonDown(unsigned char button,
                                         const glm::vec2& cursor_pos) {
  UNUSED(button);
  UNUSED(cursor_pos);

  return 0;
}
