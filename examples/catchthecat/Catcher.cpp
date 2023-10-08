#include "Catcher.h"
#include "World.h"
#include <queue>

// std::unordered_map<Point2D, std::pair<int, Point2D>> getAllPaths(World* world, Point2D p);

int getCatDirection(Point2D curCatPos, Point2D lastCatPos);

Point2D Catcher::Move(World* world) {
  auto side = world->getWorldSideSize() / 2;

  Point2D blockTile;

  blockTile = getBestTile(getAllPaths(world, world->getCat()), world, lastCatPos);

  lastCatPos = world->getCat();

  return blockTile;
}

Point2D Catcher::getBestTile(std::unordered_map<Point2D, std::pair<int, Point2D>> tilePath, World* w, Point2D lcp) {
  // Block off 2 tiles in direction cat was going
  int returnPathCost = INT_MAX;
  int sideOver2 = w->getWorldSideSize() / 2;
  std::unordered_map<Point2D, std::pair<int, Point2D>>::iterator it = tilePath.end();
  std::unordered_map<Point2D, std::pair<int, Point2D>>::iterator bestPath = tilePath.end();

  for (int i = -sideOver2; i < sideOver2; i++) {
    it = tilePath.find(Point2D(-sideOver2, i));

    if (it != tilePath.end() && returnPathCost > it->second.first) {
      returnPathCost = it->second.first;
      bestPath = it;
    }

    it = tilePath.find(Point2D(sideOver2, i));

    if (it != tilePath.end() && returnPathCost > it->second.first) {
      returnPathCost = it->second.first;
      bestPath = it;
    }

    it = tilePath.find(Point2D(i, -sideOver2));

    if (it != tilePath.end() && returnPathCost > it->second.first) {
      returnPathCost = it->second.first;
      bestPath = it;
    }

    it = tilePath.find(Point2D(i, sideOver2));

    if (it != tilePath.end() && returnPathCost > it->second.first) {
      returnPathCost = it->second.first;
      bestPath = it;
    }
  }

  if (getVisitableNeighbors(w, w->getCat()).size() == 6 && bestPath != tilePath.end()) {
    bestPath = tilePath.find(bestPath->second.second);
  }

  if (bestPath == tilePath.end()) {
    std::vector<Point2D> catNeighbors = getVisitableNeighbors(w, w->getCat());
    std::vector<Point2D> validNeighbors;

    for (Point2D neighbor : catNeighbors) {
      if (!w->getContent(neighbor)) validNeighbors.push_back(neighbor);
    }

    return validNeighbors[Random::Range(0, validNeighbors.size() - 1)];
  }

  return bestPath->first;
  // Tried getting a cost of 2, and closing that off
  // for (auto it = tilePath.begin(); it != tilePath.end(); ++it) {
  //  if (it->second.first == 2) return it->first;
  //}
}

int getCatDirection(Point2D curCatPos, Point2D lastCatPos) {
  if (curCatPos == World::NE(lastCatPos)) return 0;
  if (curCatPos == World::NW(lastCatPos)) return 1;
  if (curCatPos == World::W(lastCatPos)) return 2;
  if (curCatPos == World::SW(lastCatPos)) return 3;
  if (curCatPos == World::SE(lastCatPos)) return 4;
  if (curCatPos == World::E(lastCatPos)) return 5;

  return -1;
}

std::vector<Point2D> Catcher::getVisitableNeighbors(World* w, Point2D p) {
  std::vector<Point2D> visitableNeighbors;

  if (w->isValidPosition(World::NE(p)) && !w->getContent(World::NE(p)) && World::NE(p) != w->getCat()) visitableNeighbors.push_back(World::NE(p));
  if (w->isValidPosition(World::NW(p)) && !w->getContent(World::NW(p)) && World::NW(p) != w->getCat()) visitableNeighbors.push_back(World::NW(p));
  if (w->isValidPosition(World::E(p)) && !w->getContent(World::E(p)) && World::E(p) != w->getCat()) visitableNeighbors.push_back(World::E(p));
  if (w->isValidPosition(World::W(p)) && !w->getContent(World::W(p)) && World::W(p) != w->getCat()) visitableNeighbors.push_back(World::W(p));
  if (w->isValidPosition(World::SW(p)) && !w->getContent(World::SW(p)) && World::SW(p) != w->getCat()) visitableNeighbors.push_back(World::SW(p));
  if (w->isValidPosition(World::SE(p)) && !w->getContent(World::SE(p)) && World::SE(p) != w->getCat()) visitableNeighbors.push_back(World::SE(p));

  return visitableNeighbors;
}

std::vector<Point2D> Catcher::getUnvisitableNeighbors(World* w, Point2D p) {
  std::vector<Point2D> unvisitableNeighbors;

  if (w->isValidPosition(World::NE(p)) && w->getContent(World::NE(p))) unvisitableNeighbors.push_back(World::NE(p));
  if (w->isValidPosition(World::NW(p)) && w->getContent(World::NW(p))) unvisitableNeighbors.push_back(World::NW(p));
  if (w->isValidPosition(World::E(p)) && w->getContent(World::E(p))) unvisitableNeighbors.push_back(World::E(p));
  if (w->isValidPosition(World::W(p)) && w->getContent(World::W(p))) unvisitableNeighbors.push_back(World::W(p));
  if (w->isValidPosition(World::SW(p)) && w->getContent(World::SW(p))) unvisitableNeighbors.push_back(World::SW(p));
  if (w->isValidPosition(World::SE(p)) && w->getContent(World::SE(p))) unvisitableNeighbors.push_back(World::SE(p));

  return unvisitableNeighbors;
}

std::unordered_map<Point2D, std::pair<int, Point2D>> Catcher::getAllPaths(World* world, Point2D p) {
  std::priority_queue<std::pair<int, Point2D>, std::vector<std::pair<int, Point2D>>, std::greater<std::pair<int, Point2D>>> tileQueue;

  std::unordered_map<Point2D, std::pair<int, Point2D>> tilePath;
  // cat will find the path with least neighbors, and shortest (dijkstra)
  // neighbors become exponentially worse (num bad neighbors squared will be cost)
  tileQueue.push(std::make_pair(0, p));
  tilePath.insert(std::make_pair(Point2D(), std::make_pair(0, p)));

  for (Point2D neighbor : getVisitableNeighbors(world, p)) {
    if (world->catWinsOnSpace(neighbor)) {
      tilePath.insert(std::make_pair(neighbor, std::make_pair(0, p)));
      return tilePath;
    }
  }

  while (!tileQueue.empty()) {
    Point2D tile = tileQueue.top().second;
    int tileCost = tileQueue.top().first;
    tileQueue.pop();

    std::vector<Point2D> unvisitedNeighbors = getVisitableNeighbors(world, tile);

    for (Point2D neighbor : unvisitedNeighbors) {
      // Weights neighbors on an exponential scale (more neighbors means excessively worse)
      int unvisitableNeighborCount = getUnvisitableNeighbors(world, neighbor).size();
      int neighborCost = unvisitableNeighborCount;
      neighborCost += tileCost + 1;

      auto data = tilePath.find(neighbor);

      if (data == tilePath.end()) {
        tileQueue.push(std::make_pair(neighborCost, neighbor));
        tilePath.insert(std::make_pair(neighbor, std::make_pair(neighborCost, tile)));
      } else if (data->second.first > neighborCost) {
        tilePath[neighbor] = std::make_pair(neighborCost, tile);
      }
    }
  }

  return tilePath;
}