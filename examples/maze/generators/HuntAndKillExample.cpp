#include "HuntAndKillExample.h"
#include "../World.h"
#include "Random.h"
#include <climits>

#define FRONTIRE Color::BlueViolet
#define UNVISITED Color::DarkGray
#define VISITED Color::Black

bool HuntAndKillExample::Step(World* w) {
  auto sideOver2 = w->GetSize() / 2;

  if (stack.empty()) {
    Point2D startPos = randomStartPoint(w);
    w->SetNodeColor(startPos, FRONTIRE);
    stack.push_back(startPos);
    return false;
  }

  // Searching
  auto cameFrom = stack.back();
  auto frontier = getVisitables(w, cameFrom);

  // if no frontier, kill
  if (frontier.size() == 0) {
    for (auto point : stack) {
      w->SetNodeColor(point, VISITED);
      visited[point.y][point.x] = true;
    }
    stack.clear();
    Point2D startPoint = randomStartPoint(w);

    if (startPoint.x > w->GetSize()) return false;

    std::vector startNeighbors = getVisitedNeighbors(w, startPoint);
    if (!startNeighbors.empty()) deactivateWallInDirection(w, startPoint, startNeighbors[Random::Range(0, startNeighbors.size() - 1)] - startPoint);

    stack.push_back(startPoint);
    w->SetNodeColor(startPoint, FRONTIRE);
  } else {
    auto random_index = Random::Range(0, frontier.size() - 1);
    auto goingTo = frontier[random_index];
    w->SetNodeColor(goingTo, FRONTIRE);

    auto direction = goingTo - cameFrom;

    deactivateWallInDirection(w, cameFrom, direction);

    stack.push_back(goingTo);
  }

  return true;
}

void HuntAndKillExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D HuntAndKillExample::randomStartPoint(World* world) {
  // Todo: improve this if you want
  auto sideOver2 = world->GetSize() / 2;

  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> HuntAndKillExample::getVisitables(World* w, const Point2D& p) {
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

std::vector<Point2D> HuntAndKillExample::getVisitedNeighbors(World* w, const Point2D& p) {
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

void HuntAndKillExample::deactivateWallInDirection(World* w, const Point2D& p, const Point2D& direction) {
  // Set neighbor wall inactive
  if (direction == Point2D::UP) {
    w->SetNorth(p, false);
  } else if (direction == Point2D::LEFT) {
    w->SetWest(p, false);
  } else if (direction == Point2D::DOWN) {
    w->SetSouth(p, false);
  } else if (direction == Point2D::RIGHT) {
    w->SetEast(p, false);
  }
}