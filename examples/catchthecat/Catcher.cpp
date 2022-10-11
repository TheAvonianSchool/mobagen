#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {

  auto maybePath = pathfinder.aStar(world->getCat(), world);

  if(maybePath.has_value()) {
    auto path = maybePath.value();

    Point2D currentNode{999,999};
    while(!path.empty()) {
      if(abs(path.top().x) > 800) {
        break;
      }
      currentNode = Point2D(path.top());
      path.pop();
    }
    if(currentNode == world->getCat()) {
      return {-999,999};
    }
    return currentNode;
  }

  return {-999,999};
  auto side = world->getWorldSideSize() / 2;
  for (;;) {
    Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
    auto cat = world->getCat();
    if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  }
}
