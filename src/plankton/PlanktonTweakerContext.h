/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef PLANKTON_TWEAKER_CONTEXT_H_
#define PLANKTON_TWEAKER_CONTEXT_H_

struct PlanktonTweakerContext {
  int system_actual_fps;
  float system_time_speed;

  PlanktonTweakerContext();
  ~PlanktonTweakerContext();
};

extern PlanktonTweakerContext tweaker_ctx;

#endif /* PLANKTON_TWEAKER_CONTEXT_H_ */
