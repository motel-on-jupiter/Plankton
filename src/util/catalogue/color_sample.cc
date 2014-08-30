/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "color_sample.h"

glm::vec3 WebColor::to_fvec(const unsigned char (&color)[3]) {
  return glm::vec3(static_cast<float>(color[0]),
                   static_cast<float>(color[1]),
                   static_cast<float>(color[2])) / glm::vec3(255.0f);
}

glm::vec3 X11Color::to_fvec(const unsigned char (&color)[3]) {
  return glm::vec3(static_cast<float>(color[0]),
                   static_cast<float>(color[1]),
                   static_cast<float>(color[2])) / glm::vec3(255.0f);
}
