#include "JohnConway.h"

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  for (int x = 0; x < world.SideSize(); x++) {
    for (int y = 0; y < world.SideSize(); y++) {
      auto curPoint = Point2D(x, y);

      if (world.Get(curPoint)) {
        if (CountNeighbors(world, curPoint) < 2)
          world.SetNext(curPoint, false);
        else if (CountNeighbors(world, curPoint) < 4)
          world.SetNext(curPoint, true);
        else
          world.SetNext(curPoint, false);
      } else {
        if (CountNeighbors(world, curPoint) == 3) world.SetNext(curPoint, true);
      }
    }
  }

  world.SwapBuffers();
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  int numNeighbors = 0;

  numNeighbors += world.Get(point.Up()) ? 1 : 0;
  numNeighbors += world.Get(point.Left()) ? 1 : 0;
  numNeighbors += world.Get(point.Down()) ? 1 : 0;
  numNeighbors += world.Get(point.Right()) ? 1 : 0;
  numNeighbors += world.Get(point.Down().Right()) ? 1 : 0;
  numNeighbors += world.Get(point.Up().Right()) ? 1 : 0;
  numNeighbors += world.Get(point.Down().Left()) ? 1 : 0;
  numNeighbors += world.Get(point.Up().Left()) ? 1 : 0;

  return numNeighbors;
}
