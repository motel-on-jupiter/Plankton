/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef ENTITYDRAW_H_
#define ENTITYDRAW_H_

#include <vector>
#include "entity/BaseEntity.h"
#include "util/wrapper/glm_wrap.h"

class BaseEntityDraw : public EntityMixIn {
 public:
  BaseEntityDraw(BaseEntity &entity) : EntityMixIn(entity) {}
  virtual ~BaseEntityDraw() {}

  virtual int Initialize() = 0;
  virtual void Finalize() = 0;
  virtual void Draw() = 0;
};

class SphereEntityDraw : public BaseEntityDraw {
 public:
  SphereEntityDraw(BaseEntity &entity, const glm::vec3 &ambient, const glm::vec3 &difusse,
                   const glm::vec3 &specular, float shiness, int slices, int stacks);
  SphereEntityDraw(BaseEntity &entity, const glm::vec3 &ambient_and_difusse, const glm::vec3 &specular,
                   float shiness, int slices, int stacks);
  virtual ~SphereEntityDraw() {}

  virtual int Initialize();
  virtual void Finalize();
  virtual void Draw();

 private:
  GLUquadric *quadric_;
  std::vector<glm::vec3> colors_;
  float shiness_;
  int slices_;
  int stacks_;
};

#endif /* ENTITYDRAW_H_ */
