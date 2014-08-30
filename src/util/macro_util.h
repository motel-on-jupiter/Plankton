/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef MACROUTIL_H_
#define MACROUTIL_H_

#ifndef UNUSED
#define UNUSED(_arg) do { (void)(_arg); } while(0)
#endif

#ifdef _MSC_VER
#include <Windows.h>
#else
#ifndef ARRAYSIZE
#define ARRAYSIZE(_var) (sizeof(_var) / sizeof((_var)[0]))
#endif
#endif

#endif /* MACROUTIL_H_ */
