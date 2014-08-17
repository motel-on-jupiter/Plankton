/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef ENTITYROUTING_H_
#define ENTITYROUTING_H_

#include "entity/BaseEntity.h"

class EntityRouting : public EntityMixIn {
public:
  EntityRouting(BaseEntity &entity, const WaypointGraph &graph,
                const Waypoint &origin, const Waypoint &terminus,
                float movespeed, float turnspeed);
  ~EntityRouting() {}

  void Update(float elapsed_time);
  void Reroute(const Waypoint &terminus);
  bool HasReached() const { return goal_ == nullptr; }

  Navigator &navi() { return navi_; }
  float movespeed() const { return movespeed_; }
  float turnspeed() const { return turnspeed_; }
  const Waypoint *goal() const { return goal_; }
  const Waypoint *lastgoal() const { return lastgoal_; }

private:
  Navigator navi_;
  const Waypoint *goal_;
  const Waypoint *lastgoal_;
  float movespeed_;
  float turnspeed_;
};

#endif /* ENTITYROUTING_H_ */
