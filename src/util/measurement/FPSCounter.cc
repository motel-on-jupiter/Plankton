/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "FPSCounter.h"
#include <cmath>

FPSCounter::FPSCounter(int sampling_msec)
: sampling_msec_(sampling_msec),
  fps_(0),
  count_(0),
  count_start_msec(0) {
}

void FPSCounter::Update(int current_msec) {
  // Count frames
  ++count_;

  // Update FPS
  int elapsed_msec = current_msec - count_start_msec;
  if (elapsed_msec > sampling_msec_) {
    fps_ = count_ * static_cast<int>(roundf(1000.0f / static_cast<float>(sampling_msec_)));
    count_ = 0;
    count_start_msec = current_msec;
  }
}
