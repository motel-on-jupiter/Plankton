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

  GLuint GetUniformLocation(const GLchar *uniform) const;
  void SetUniform1f(const GLchar *uniform, GLfloat v0) const;
  void SetUniform2f(const GLchar *uniform, GLfloat v0, GLfloat v1) const;
  void SetUniform3f(const GLchar *uniform, GLfloat v0, GLfloat v1, GLfloat v2) const;
  void SetUniform4f(const GLchar *uniform, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
  void SetUniform1i(const GLchar *uniform, GLint v0) const;
  void SetUniform2i(const GLchar *uniform, GLint v0, GLint v1) const;

  GLuint name() const {
    return name_;
  }

 private:
  std::vector<GLuint> shaders_;
  GLuint name_;
};

#endif /* GL_SHADER_H_ */
