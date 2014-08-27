/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SPECTRUMWAVESCENE_H_
#define SPECTRUMWAVESCENE_H_

#include <vector>
#include "plankton/PlanktonGame.h"

class SpectrumWaveScene : public PlanktonGameSceneInterface {
 public:
  SpectrumWaveScene();
  virtual ~SpectrumWaveScene() {}

  virtual int Initialize(const glm::vec2 &window_size);
  virtual void Finalize();
  virtual void Update(float elapsed_time, const glm::vec2 &window_size);
  virtual void Draw(const glm::vec2 &window_size);
  virtual int OnMouseButtonDown(unsigned char button, const glm::vec2 &cursor_pos);

 private:
  static const size_t kNumBars;

  bool initialize_;
  std::vector<float> bar_lengths_;
  float remain_time_;
};
#endif /* SPECTRUMWAVESCENE_H_ */
