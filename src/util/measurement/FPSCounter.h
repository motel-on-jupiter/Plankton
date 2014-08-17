/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef FPSCOUNTER_H_
#define FPSCOUNTER_H_

class FPSCounter {
 public:
  FPSCounter(int sampling_msec);
  ~FPSCounter() {}

  void Update(int current_msec);

  int sampling_msec() const { return sampling_msec_; }
  int fps() const { return fps_; }

 private:
  int sampling_msec_;
  int fps_;
  int count_;
  int count_start_msec;
};

#endif /* FPSCOUNTER_H_ */
