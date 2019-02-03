/*
------------------------------------------------------------------------------------------------------------------------
Copyright 2019 Vite Falcon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------------------------------
*/

#include "MovementSystem.h"

#include <Urho3D/Core/CoreEvents.h>

#include "../components/AngularVelocity.h"
#include "../components/Direction.h"
#include "../components/Position.h"
#include "../components/Velocity.h"

#include "../events/UpdateEventData.h"

MovementSystem::MovementSystem(Urho3D::Context *context,
                               Urho3D::EntityRegistry &registry)
    : Urho3D::EntitySystem(context, registry) {}

void MovementSystem::Update(UpdateEventData &data,
                            Urho3D::EntityRegistry &registry) {
  float dt = data.GetTimeStep();

  auto rotatingEntities = registry.view<Direction, AngularVelocity>();
  for (auto entity : rotatingEntities) {
    auto &velocity = registry.get<AngularVelocity>(entity).value;
    Urho3D::Quaternion deltaRotation = Urho3D::Quaternion(
        velocity.x_ * dt, velocity.y_ * dt, velocity.z_ * dt);
    registry.get<Direction>(entity).Rotate(deltaRotation);
  }

  auto translatingEntities = registry.view<Position, Velocity>();
  for (auto entity : translatingEntities) {
    auto &velocity = registry.get<Velocity>(entity).value;
    registry.get<Position>(entity).value += velocity * dt;
  }
}
