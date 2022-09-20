//
// Created by miste on 20-Sep-22.
//

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Point2D.h"
#include <hash_map>
#include <set>

class Pathfinder {
  std::hash_map<int32_t, std::set<Point2D>> graphMap;
  std::vector<Point2D> visited;
};

#endif  // PATHFINDER_H
