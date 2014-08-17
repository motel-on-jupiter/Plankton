/**
 * Copyright (C) 2014 The Motel On Jupiter
 */
#ifndef SPIRITFLOATINGSCENE_H_
#define SPIRITFLOATINGSCENE_H_

#include "entity/BaseEntity.h"
#include "plankton/PlanktonGame.h"

class Spirit : public BaseEntity {
 public:
  Spirit() : BaseEntity(), quadric_(nullptr) {}
  ~Spirit() {}

  virtual int Initialize();
  virtual void Finalize();
  virtual void Update(float elapsed_time);
  virtual void Draw();

 private:
  GLUquadric *quadric_;
};

class SpiritFloatingScene : public PlanktonGameSceneInterface {
 public:
  SpiritFloatingScene();
  virtual ~SpiritFloatingScene() {}

  virtual int Initialize();
  virtual void Finalize();
  virtual void Update(float elapsed_time);
  virtual void Draw();
  virtual int OnMouseButtonDown(unsigned char button, const glm::vec2 &cursor_pos);

 private:
  bool initialize_;
  Spirit spirit_;
};
#endif /* SPIRITSCENE_H_ */
