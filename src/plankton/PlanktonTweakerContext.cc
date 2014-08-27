/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#include "plankton/PlanktonTweakerContext.h"

PlanktonTweakerContext::PlanktonTweakerContext()
: system_actual_fps(0),
  system_time_speed(1.0f) {
}

PlanktonTweakerContext::~PlanktonTweakerContext() {
}

PlanktonTweakerContext tweaker_ctx;
