
#include "Pathfinder.h"
#include "World.h"

const Point2D Pathfinder::TARGET_GOAL = {999, 999};

void Pathfinder::addNode(Point2D node, const std::unordered_set<Point2D, PointHash::Hasher>& neighbors) {
  graphMap.insert({node, neighbors});
}

// https://en.wikipedia.org/wiki/A*_search_algorithm
std::optional<std::stack<Point2D>> Pathfinder::aStar(Point2D start, World* world, SDL_Renderer* renderer) {

  if(graphMap.empty() || worldSize != world->getWorldSideSize()) {
    generateMap(world);
    worldSize = world->getWorldSideSize();
  }

  bool visualize = renderer != nullptr;

  // this doesn't account for diagonals but that can be fixed with more spaghetti

  auto heuristicFunction = [world](Point2D point) -> int {
    int edge = world->getWorldSideSize() / 2;

    return edge - abs(abs(point.x) > abs(point.y) ? point.x : point.y);
  };

  auto compareHeuristics = [heuristicFunction](Point2D point, Point2D other) -> bool {
    return heuristicFunction(point) > heuristicFunction(other);
  };

  auto reconstructPath = [](const PointMap<Point2D>& came, Point2D cur) -> std::stack<Point2D> {


    std::stack<Point2D> path;
    path.push(cur);

    Point2D current = cur;

    // while the current node is still a key
    // for some reason find didnt work...
    while(came.count(current) > 0/*came.find(current) != came.end()*/) {
      current = came.at(current);
      path.push(current);
    }
    return path;
  };

  // visited nodes
  // https://stackoverflow.com/questions/16111337/declaring-a-priority-queue-in-c-with-a-custom-comparator
  PriorityQueue<Point2D, std::vector<Point2D>, decltype(compareHeuristics)> openSet(compareHeuristics);

  openSet.push(start);

  // cameFrom[point] is previous node in cheap path
  PointMap<Point2D> cameFrom;

  // Currently cheapest path from start to point
  PointMap<int> gScore;

  gScore.insert({start, 0});

  // fScore[point] = best guess of work through this point
  // to finish (gScore[point] + heuristic(point))
  PointMap<int> fScore;

  fScore.insert({start, heuristicFunction(start)});

  while (!openSet.empty()) {
    Point2D current = openSet.top();
    if (current == TARGET_GOAL) {
      return {reconstructPath(cameFrom, current)};
    }

    openSet.pop();

    for (auto& neighbor : graphMap[current]) {
      if (world->isValidPosition(neighbor) && world->getContent(neighbor)) {
        continue;
      }

      int currentGScore = gScore[current] + 1;
      if (currentGScore < (gScore.extract(neighbor).empty() ? std::numeric_limits<int>::max() : gScore[neighbor])) {
        cameFrom.insert({neighbor, current});
        gScore.insert({neighbor, currentGScore});
        fScore.insert({neighbor, currentGScore + heuristicFunction(neighbor)});

        if (auto f = openSet.find(neighbor); f == openSet.end()) {
          openSet.push(neighbor);
        }
      }
    }
  }

  return {};
}

void Pathfinder::generateMap(World* world) {
  graphMap.clear();

  // for world size
  // get neighbors of point
  // make point switch from every direction
  // for point in 0..6
  // ne e se sw w nw
  // if point is outside map make it 999, 999
  // if inside world simply add point
  auto getDirection = [](int index) {
    switch(index) {
      default:
        [[fallthrough]];
      case 0:
        return World::NE;
      case 1:
        return World::E;
      case 2:
        return World::SE;
      case 3:
        return World::SW;
      case 4:
        return World::W;
      case 5:
        return World::NW;
    }
  };

  const int DIRECTIONS = 6;
  int halfSize = world->getWorldSideSize() / 2;
  for(int x = -halfSize; x <= halfSize; ++x) {
    for(int y = -halfSize; y <= halfSize; ++y) {
      std::unordered_set<Point2D, PointHash::Hasher> neighbors;
      Point2D point{x,y};
      for(int direction = 0; direction < DIRECTIONS; ++direction) {
        Point2D neighbor = getDirection(direction)(point);

        if(!world->isValidPosition(neighbor)) {
          neighbor = TARGET_GOAL;
        }
        neighbors.insert(neighbor);
      }
      addNode(point, neighbors);
    }
  }
}
