#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  Vector2f cohesionForce;

  if (neighborhood.size() <= 0) return cohesionForce;

  for (Boid* neighbor : neighborhood) {
    cohesionForce += neighbor->getPosition();
  }

  cohesionForce = cohesionForce / neighborhood.size();
  cohesionForce = cohesionForce - boid->getPosition();
  // todo: add your code here to make a force towards the center of mass
  // hint: iterate over the neighborhood
  // find center of mass

  // cohesionForce = Vector2f::normalized(cohesionForce);

  return cohesionForce.normalized();
}