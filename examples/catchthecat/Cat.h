#ifndef CAT_H
#define CAT_H

#include "Agent.h"
#include "Pathfinder.h"

class Cat : public Agent {
  Pathfinder pathfinder;
 public:
  explicit Cat() : Agent(){}
  Point2D Move(World*) override;
};

#endif  // CAT_H
