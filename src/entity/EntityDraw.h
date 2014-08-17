/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef ENTITYDRAW_H_
#define ENTITYDRAW_H_

#include "entity/BaseEntity.h"

class EntityDraw : public EntityMixIn {
 public:
  EntityDraw(BaseEntity &entity) : EntityMixIn(entity) {}
  virtual ~EntityDraw() {}

  virtual void Draw() = 0;
};

#endif /* ENTITYDRAW_H_ */
