#include "AlignmentRule.h"
#include "../gameobjects/Boid.h"

Vector2f AlignmentRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to match the heading of neighbors = Average velocity
  Vector2f averageVelocity;

  if (neighborhood.size() <= 0) return averageVelocity;

  for (Boid* neighbor : neighborhood) {
    averageVelocity += neighbor->getVelocity();
  }

  averageVelocity = averageVelocity / neighborhood.size();

  // todo: add your code here to align each boid in a neighborhood
  // hint: iterate over the neighborhood

  return averageVelocity.normalized();
}