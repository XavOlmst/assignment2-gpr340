#include "Cat.h"
#include "World.h"
#include <queue>
#include <vector>


//Would make these 3 member functions of agent if allowed to chnage agent (idk if we are or not)




Point2D Cat::Move(World* world) {
  auto pos = world->getCat();

  return getBestTile(getAllPaths(world, pos), world, pos);
}

Point2D Cat::getBestTile(std::unordered_map<Point2D, std::pair<int, Point2D>> tilePath, World* w, Point2D p) {
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

  if (bestPath == tilePath.end()) {
    int dir = Random::Range(0, 5);

    while (true) {
      switch (dir) {
        case 0:
          if (w->catCanMoveToPosition(World::NE(p))) return World::NE(p);
          break;
        case 1:
          if (w->catCanMoveToPosition(World::NW(p))) return World::NW(p);
          break;
        case 2:
          if (w->catCanMoveToPosition(World::W(p))) return World::W(p);
          break;
        case 3:
          if (w->catCanMoveToPosition(World::SW(p))) return World::SW(p);
          break;
        case 4:
          if (w->catCanMoveToPosition(World::SE(p))) return World::SE(p);
          break;
        case 5:
          if (w->catCanMoveToPosition(World::E(p))) return World::E(p);
          break;
      }

      dir = Random::Range(0, 5);
    }
  }

  while (bestPath->second.second != p) {
    bestPath = tilePath.find(bestPath->second.second);
  }

  return bestPath->first;
}

std::vector<Point2D> Cat::getVisitableNeighbors(World* w, Point2D p) {
  std::vector<Point2D> visitableNeighbors;

  if (w->isValidPosition(World::NE(p)) && !w->getContent(World::NE(p))) visitableNeighbors.push_back(World::NE(p));
  if (w->isValidPosition(World::NW(p)) && !w->getContent(World::NW(p))) visitableNeighbors.push_back(World::NW(p));
  if (w->isValidPosition(World::E(p)) && !w->getContent(World::E(p))) visitableNeighbors.push_back(World::E(p));
  if (w->isValidPosition(World::W(p)) && !w->getContent(World::W(p))) visitableNeighbors.push_back(World::W(p));
  if (w->isValidPosition(World::SW(p)) && !w->getContent(World::SW(p))) visitableNeighbors.push_back(World::SW(p));
  if (w->isValidPosition(World::SE(p)) && !w->getContent(World::SE(p))) visitableNeighbors.push_back(World::SE(p));

  return visitableNeighbors;
}

std::vector<Point2D> Cat::getUnvisitableNeighbors(World* w, Point2D p) {
  std::vector<Point2D> unvisitableNeighbors;

  if (w->isValidPosition(World::NE(p)) && w->getContent(World::NE(p))) unvisitableNeighbors.push_back(World::NE(p));
  if (w->isValidPosition(World::NW(p)) && w->getContent(World::NW(p))) unvisitableNeighbors.push_back(World::NW(p));
  if (w->isValidPosition(World::E(p)) && w->getContent(World::E(p))) unvisitableNeighbors.push_back(World::E(p));
  if (w->isValidPosition(World::W(p)) && w->getContent(World::W(p))) unvisitableNeighbors.push_back(World::W(p));
  if (w->isValidPosition(World::SW(p)) && w->getContent(World::SW(p))) unvisitableNeighbors.push_back(World::SW(p));
  if (w->isValidPosition(World::SE(p)) && w->getContent(World::SE(p))) unvisitableNeighbors.push_back(World::SE(p));

  return unvisitableNeighbors;
}

std::unordered_map<Point2D, std::pair<int, Point2D>> Cat::getAllPaths(World* world, Point2D p) {
  std::priority_queue<std::pair<int, Point2D>, std::vector<std::pair<int, Point2D>>, std::greater<std::pair<int, Point2D>>> tileQueue;

  std::unordered_map<Point2D, std::pair<int, Point2D>> tilePath;
  // cat will find the path with least neighbors, and shortest (dijkstra)
  // neighbors become exponentially worse (num bad neighbors squared will be cost)
  tileQueue.push(std::make_pair(0, p));
  tilePath.insert(std::make_pair(Point2D(), std::make_pair(0, p)));

  //Check if the current neighbors have a winning tile
  for (Point2D neighbor : getVisitableNeighbors(world, p))
  {
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