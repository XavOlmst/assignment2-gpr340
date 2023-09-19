//
// Created by atolstenko on 2/9/2023.
//

#include "HexagonGameOfLife.h"
void HexagonGameOfLife::Step(World& world) 
{
    for (int x = 0; x < world.SideSize(); x++) 
    {
        for (int y = 0; y < world.SideSize(); y++) 
        {
            auto curPoint = Point2D(x, y);

            if (world.Get(curPoint))
            {
              if (CountNeighbors(world, curPoint) <= 1) 
                  world.SetNext(curPoint, false);

              if (CountNeighbors(world, curPoint) >= 3) 
                  world.SetNext(curPoint, false);

              if (CountNeighbors(world, curPoint) == 2) 
                  world.SetNext(curPoint, true);
            }
            else
            {
              if (CountNeighbors(world, curPoint) == 2) 
                  world.SetNext(curPoint, true);
            }
        }
    }

    world.SwapBuffers();
}
int HexagonGameOfLife::CountNeighbors(World& world, Point2D point) {
  int numNeighbors = 0;
	
  bool yIsOdd = point.y % 2;

  if (yIsOdd)
  {
        numNeighbors += world.Get(point.Up().Right()) ? 1 : 0;
        numNeighbors += world.Get(point.Down().Right()) ? 1 : 0;
  }
  else
  {
        numNeighbors += world.Get(point.Up().Left()) ? 1 : 0;
        numNeighbors += world.Get(point.Down().Left()) ? 1 : 0;
  }
  numNeighbors += world.Get(point.Up()) ? 1 : 0;
  numNeighbors += world.Get(point.Right()) ? 1 : 0;
  numNeighbors += world.Get(point.Down()) ? 1 : 0;

  numNeighbors += world.Get(point.Left()) ? 1 : 0;

  return numNeighbors;
}
