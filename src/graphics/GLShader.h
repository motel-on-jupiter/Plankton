/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef GL_SHADER_H_
#define GL_SHADER_H_

class GLShader {
 public:
  GLShader(GLenum type, const char *path);
  ~GLShader();

  int Compile();

  GLenum type() const {
    return type_;
  }
  const char* path() const {
    return path_;
  }
  GLuint name() const {
    return name_;
  }

 private:
  GLenum type_;
  const char *path_;
  GLuint name_;
};

class GLShaderProgram {
 public:
  GLShaderProgram();
  ~GLShaderProgram();

  void PushShader(const GLShader &shader);
  int Link();
  void Clean();

  GLuint name() const {
    return name_;
  }

 private:
  std::vector<GLuint> shaders_;
  GLuint name_;
};

#endif /* GL_SHADER_H_ */
