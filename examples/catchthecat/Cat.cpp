#include "Cat.h"
#include "World.h"

Point2D Cat::Move(World* world) {
  auto catPos = world->getCat();

  auto bestMoves = pathfinder.aStar(catPos, world);
  if(bestMoves.has_value()) {
    if(bestMoves->top() == Pathfinder::TARGET_GOAL) {
      // cat win temp
      return {0, 0};
    }
    // the first move will always be the current position
    bestMoves->pop();
    return bestMoves->top();
  }
  // cat destination: 999, 999
  // cat win: 999, -999
  // cat lose: -999, -999

  // no moves cat can make (cat lose)
  return {-999, -999};
}