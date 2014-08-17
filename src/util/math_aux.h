/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef MATHAUX_H_
#define MATHAUX_H_

#include "util/wrapper/glm_wrap.h"

inline int    sign_i(int value)   { return (value > 0) - (value < 0); }
inline float  sign_f(float value) { return static_cast<float>((value > 0.0f) - (value < 0.0f)); }
inline double sign(double value)  { return static_cast<double>((value > 0.0) - (value < 0.0)); }

inline float  saturate_f(float value) { return (value > 1.0f) ? 1.0f : (value < 0.0f) ? 0.0f : value; }
inline double saturate(double value)  { return (value > 1.0) ? 1.0 : (value < 0.0) ? 0.0 : value; }

inline bool is_fequal(float lvalue, float rvalue) { return glm::epsilonEqual(lvalue, rvalue, glm::epsilon<float>()); }
inline bool is_fzero(float value) { return is_fequal(value, 0.0f); }

inline float normalize_angle(float angle) {
  while (angle > glm::pi<float>())         { angle -= glm::pi<float>() * 2.0f; }
  while (angle < glm::pi<float>() * -1.0f) { angle += glm::pi<float>() * 2.0f; }
  return angle;
}

#endif /* MATHAUX_H_ */
