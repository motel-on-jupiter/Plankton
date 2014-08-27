/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef MALLTWEAKERCONTEXT_H_
#define MALLTWEAKERCONTEXT_H_

struct PlanktonTweakerContext {
  int system_actual_fps;
  float system_time_speed;

  PlanktonTweakerContext();
  ~PlanktonTweakerContext();
};

extern PlanktonTweakerContext tweaker_ctx;

#endif /* MALLTWEAKERCONTEXT_H_ */
