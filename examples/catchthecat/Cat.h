#ifndef CAT_H
#define CAT_H

#include "Agent.h"
#include <vector>
#include <unordered_map>

class Cat : public Agent {
public:
  explicit Cat() : Agent(){};
  Point2D Move(World*) override;

  std::unordered_map<Point2D, std::pair<int, Point2D>> getAllPaths(World* world, Point2D p);
  std::vector<Point2D> getVisitableNeighbors(World* w, Point2D p);
  std::vector<Point2D> getUnvisitableNeighbors(World* w, Point2D p);
  Point2D getBestTile(std::unordered_map<Point2D, std::pair<int, Point2D>> tilePath, World* w, Point2D p);

};

#endif  // CAT_H
