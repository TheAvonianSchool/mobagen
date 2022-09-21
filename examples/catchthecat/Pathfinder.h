
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Point2D.h"
#include "SDL.h"
#include <optional>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

class World;

namespace PointHash {
class Hasher {
 public:
  std::size_t operator()(Point2D const& key) const {
    static std::hash<int> int_hasher;
    const int RAND_PRIME = 73856093;
    return (int_hasher(key.x) + RAND_PRIME) * RAND_PRIME + int_hasher(key.y);
  }
};

class EqualFn {
 public:
  bool operator()(Point2D const& t1, Point2D const& t2) const {
    return t1 == t2;
  }
};

}  // namespace PointHash

// https://stackoverflow.com/questions/16749723/how-i-can-find-value-in-priority-queue
template <
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>>
class PriorityQueue : public std::priority_queue<T, Container, Compare> {
 public:
  typedef typename std::priority_queue<
      T,
      Container,
      Compare>::container_type::const_iterator const_iterator;

  explicit PriorityQueue(const Compare& pred)
      : std::priority_queue<T, Container, Compare>(pred) {}

  const_iterator find(const T& val) const {
    auto first = this->c.cbegin();
    auto last = this->c.cend();
    while (first != last) {
      if (*first == val) return first;
      ++first;
    }
    return last;
  }

  const_iterator end() const {
    return this->c.cend();
  }
};

class Pathfinder {
  template <typename T>
  using PointMap = std::unordered_map<Point2D, T, PointHash::Hasher, PointHash::EqualFn>;

  PointMap<std::unordered_set<Point2D, PointHash::Hasher>> graphMap;


  int worldSize;

  void generateMap(World* world);

 public:
  static const Point2D TARGET_GOAL;

  void addNode(Point2D node, const std::unordered_set<Point2D, PointHash::Hasher>& neighbors);

  std::optional<std::stack<Point2D>> aStar(Point2D start, World* world, SDL_Renderer* renderer = nullptr);
};

#endif  // PATHFINDER_H
