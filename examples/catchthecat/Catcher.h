#ifndef CATCHER_H
#define CATCHER_H

#include "Agent.h"
#include <unordered_map>
#include <vector>

class Catcher : public Agent {
public:
  explicit Catcher() : Agent(){};
  Point2D Move(World*) override;

  Point2D lastCatPos = Point2D(0, 0);

  std::unordered_map<Point2D, std::pair<int, Point2D>> getAllPaths(World* world, Point2D p);
  std::vector<Point2D> getVisitableNeighbors(World* w, Point2D p);
  std::vector<Point2D> getUnvisitableNeighbors(World* w, Point2D p);
  Point2D getBestTile(std::unordered_map<Point2D, std::pair<int, Point2D>> tilePath, World* w, Point2D p);
};

#endif  // CATCHER_H
