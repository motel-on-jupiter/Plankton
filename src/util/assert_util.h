/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef ASSERT_UTIL_H_
#define ASSERT_UTIL_H_

extern void ASSERT(bool expr);
extern void assert_not_reached();
template<class T> extern void assert_equal(T n1, T n2) {
  ASSERT(n1 == n2);
}
template<class T> extern void assert_not_equal(T n1, T n2) {
  ASSERT(n1 != n2);
}
template<class T> extern void assert_null(T *expr) {
  assert_equal(expr, nullptr);
}
template<class T> extern void assert_nonnull(T *expr) {
  assert_not_equal(expr, nullptr);
}

#endif /* ASSERT_UTIL_H_ */
