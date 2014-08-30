/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef GLGRAPHICSWRAP_H_
#define GLGRAPHICSWRAP_H_

#include <GL/glew.h>
#include <GL/freeglut.h>

/**
 * To vanish the output of method-unused warning
 */
namespace freeglut_avoid_warning {
class _dummy {
  _dummy() {
    glutInit(nullptr, nullptr);
    glutCreateWindow(nullptr);
    glutCreateMenu(nullptr);
  }
};
}

/**
 * Auxiliary functions
 */
#include <vector>
extern int glCompileShaderFile(GLuint shader, const char *path);
extern int glLinkProgramWithShaders(GLuint program,
                                    std::vector<GLuint> &shaders);

#endif /* GLGRAPHICSWRAP_H_ */
