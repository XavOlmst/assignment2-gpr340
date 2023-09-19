#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to avoid boids too close
  Vector2f separatingForce;

  if (neighborhood.size() <= 0) return separatingForce;

  int closeNeighbors = 0;

  for (Boid* neighbor : neighborhood) {
    auto distance = boid->getPosition() - neighbor->getPosition();

    if (distance.getMagnitude() < desiredMinimalDistance) {
      separatingForce += neighbor->getPosition();
      closeNeighbors++;
    }
  }

  if (closeNeighbors == 0) return separatingForce;

  separatingForce = separatingForce / neighborhood.size();
  separatingForce = boid->getPosition() - separatingForce;

  //    float desiredDistance = desiredMinimalDistance;
  //
  //    // todo: implement a force that if neighbor(s) enter the radius, moves the boid away from it/them
  //    if (!neighborhood.empty()) {
  //        Vector2f position = boid->transform.position;
  //        int countCloseFlockmates = 0;
  //        // todo: find and apply force only on the closest mates
  //    }

  // separatingForce = Vector2f::normalized(separatingForce);

  return separatingForce.normalized();
}

bool SeparationRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valusHasChanged = false;

  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valusHasChanged = true;
  }

  return valusHasChanged;
}
