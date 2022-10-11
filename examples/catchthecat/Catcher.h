#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"
#include "Pathfinder.h"

class Catcher : public Agent {
  Pathfinder pathfinder;
 public:
  explicit Catcher() : Agent(){};
  Point2D Move(World*) override;
};

#endif  // CATCHER_H
