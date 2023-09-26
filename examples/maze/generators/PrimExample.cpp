#include "PrimExample.h"
#include "../World.h"
#include "Random.h"

#define FRONTIRE Color::BlueViolet
#define UNVISITED Color::DarkGray
#define VISITED Color::Black

bool PrimExample::Step(World* w) {
  int sideOver2 = w->GetSize() / 2;
  //auto visitedColor = Color::Black;
  //auto toVisitColor = Color::PaleVioletRed;

  // Suppost to do something when it's not initialized
  if (!initialized) {
    Point2D p(-sideOver2 + 1, -sideOver2);
    toBeVisited.push_back(p);
    w->SetNodeColor(p, Color::PaleVioletRed);
    initialized = true;
    return false;
  }

  auto random_index = Random::Range(0, toBeVisited.size() - 1);
  auto node = toBeVisited[random_index];

  w->SetNodeColor(node, VISITED);
  toBeVisited.erase(std::remove(toBeVisited.begin(), toBeVisited.end(), node), toBeVisited.end());

  auto visitedNeighbors = getVisitedNeighbors(w, node);

  if (visitedNeighbors.size() > 0) {
    random_index = Random::Range(0, visitedNeighbors.size() - 1);

    auto direction = visitedNeighbors[random_index] - node;

    // Set neighbor wall inactive
    if (direction == Point2D::UP && abs(node.y) <= sideOver2) {
      w->SetNorth(node, false);
    } else if (direction == Point2D::LEFT && abs(node.x) <= sideOver2) {
      w->SetWest(node, false);
    } else if (direction == Point2D::DOWN && abs(node.y) <= sideOver2) {
      w->SetSouth(node, false);
    } else if (direction == Point2D::RIGHT && abs(node.x) <= sideOver2) {
      w->SetEast(node, false);
    }
  }

  auto visitables = getVisitables(w, node);
  //toBeVisited.reserve(toBeVisited.size() + visitables.size());


  for (auto visitableNeighbor : visitables) {
    toBeVisited.push_back(visitableNeighbor);
    w->SetNodeColor(visitableNeighbor, FRONTIRE);
  }

  if (toBeVisited.empty()) return false;

  return true;
}
void PrimExample::Clear(World* world) {
  toBeVisited.clear();
  initialized = false;
}

std::vector<Point2D> PrimExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};
  std::vector<Point2D> visitables;

  // Adds all deltas as visitables
  for (auto direction : deltas) {
    Point2D nextPoint = p + direction;

    // Ensures not over on sides
    if (abs(nextPoint.x) > sideOver2 || abs(nextPoint.y) > sideOver2) continue;

    if (w->GetNodeColor(nextPoint) == UNVISITED) {
      visitables.push_back(nextPoint);
    }
  }

  return visitables;
}

std::vector<Point2D> PrimExample::getVisitedNeighbors(World* w, const Point2D& p) {
  std::vector<Point2D> deltas = {Point2D::UP, Point2D::DOWN, Point2D::LEFT, Point2D::RIGHT};
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> neighbors;

  for (Point2D direction : deltas) {
    Point2D nextPoint = p + direction;

    // Ensures not over on sides
    if (abs(nextPoint.x) > sideOver2 || abs(nextPoint.y) > sideOver2) continue;

    if (w->GetNodeColor(nextPoint) == VISITED) neighbors.push_back(nextPoint);
  }

  return neighbors;
}
