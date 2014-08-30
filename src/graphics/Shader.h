/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SHADER_H_
#define SHADER_H_

class Shader {
 public:
  Shader(GLenum type, const char *path);
  ~Shader();

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

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();

  void PushShader(const Shader &shader);
  int Link();
  void Clean();

  GLuint name() const {
    return name_;
  }

 private:
  std::vector<GLuint> shaders_;
  GLuint name_;
};

#endif /* SHADER_H_ */
