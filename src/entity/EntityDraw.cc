/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#include "EntityDraw.h"
#include "entity/BaseEntity.h"
#include "util/logging/Logger.h"
#include "util/wrapper/glgraphics_wrap.h"

SphereEntityDraw::SphereEntityDraw(BaseEntity &entity, const glm::vec3 &ambient,
                                   const glm::vec3 &difusse, const glm::vec3 &specular,
                                   float shiness, int slices, int stacks)
: BaseEntityDraw(entity), quadric_(nullptr), colors_(), shiness_(shiness), slices_(slices),
  stacks_(stacks) {
  colors_.push_back(ambient);
  colors_.push_back(difusse);
  colors_.push_back(specular);
}

SphereEntityDraw::SphereEntityDraw(BaseEntity &entity, const glm::vec3 &ambient_and_difusse,
                                   const glm::vec3 &specular, float shiness, int slices,
                                   int stacks)
: BaseEntityDraw(entity), quadric_(nullptr), colors_(), shiness_(shiness), slices_(slices),
  stacks_(stacks) {
  colors_.push_back(ambient_and_difusse);
  colors_.push_back(specular);
}

void SphereEntityDraw::Draw() {
  if (colors_.size() == 2) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glm::value_ptr(colors_[0]));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(colors_[1]));
  } else if (colors_.size() == 3) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(colors_[0]));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(colors_[1]));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(colors_[2]));
  } else {
    assert(false);
  }
  glMaterialfv(GL_FRONT, GL_SHININESS, &shiness_);

  glPushMatrix();
  glMultMatrixf(glm::value_ptr(glm::translate(entity().pos())));
  gluSphere(quadric_, entity().scale().x * 0.5f, slices_, stacks_);
  glPopMatrix();
}

int SphereEntityDraw::Initialize() {
  quadric_ = gluNewQuadric();
  if (quadric_ == nullptr) {
    LOGGER.Error("Failed to create quadric object");
    return -1;
  }
  return 0;
}

void SphereEntityDraw::Finalize() {
  gluDeleteQuadric(quadric_);
  quadric_ = nullptr;
}
