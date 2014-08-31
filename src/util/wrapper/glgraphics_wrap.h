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

extern inline void glActiveTextureUnit(GLuint unit);
extern inline void glBindDrawFramebuffer(GLuint framebuffer);
extern inline void glBindReadFramebuffer(GLuint framebuffer);
extern inline void glBindRenderbuffer_(GLuint renderbuffer);
extern inline void glBindTexture1D(GLuint texture);
extern inline void glBindTexture2D(GLuint texture);
extern inline void glBindTexture3D(GLuint texture);
extern inline GLenum glCheckDrawFramebufferStatus();
extern inline GLenum glCheckReadFramebufferStatus();
extern inline void glClearAll();
extern inline void glDrawFramebufferColorTexture(GLint attachidx,
                                                 GLuint texture, GLint level);
extern inline void glDrawFramebufferDepthTexture(GLuint texture, GLint level);
extern inline void glDrawFramebufferRenderbuffer(GLenum attachment,
                                                 GLuint renderbuffer);
extern inline void glFramebufferColorTexture(GLenum target, GLint attachidx,
                                             GLuint texture, GLint level);
extern inline void glFramebufferDepthTexture(GLenum target, GLuint texture,
                                             GLint level);
extern inline void glFramebufferDrawColorAttachment(GLint attachidx);
extern inline void glGenFramebuffer(GLuint *id);
extern inline void glGenRenderbuffer(GLuint *renderbuffer);
extern inline void glGenTexture(GLuint *texture);
extern inline void glReadFramebufferColorTexture(GLint attachidx,
                                                 GLuint texture, GLint level);
extern inline void glReadFramebufferDepthTexture(GLuint texture, GLint level);
extern inline void glReadFramebufferRenderbuffer(GLenum attachment,
                                                 GLuint renderbuffer);
extern inline void glUnbindDrawFramebuffer();
extern inline void glUnbindFramebuffer(GLenum target);
extern inline void glUnbindReadFramebuffer();

/**
 * Renamed functions
 */
extern inline void glSetClearingColor(GLclampf red, GLclampf green,
                                      GLclampf blue, GLclampf alpha);

#endif /* GLGRAPHICSWRAP_H_ */
