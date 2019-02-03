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

#include "DemoState.h"

#include "../components/AngularVelocity.h"
#include "../components/Direction.h"
#include "../components/Name.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Scale.h"
#include "../components/Skybox.h"
#include "../components/Sound.h"

#include "../events/GameEvents.h"

#include "../components/Camera.h"
#include "../components/Light.h"
#include "../components/SoundListener.h"
#include "../components/StaticModel.h"
#include "../components/Velocity.h"

#include <Urho3D/Input/Input.h>

DemoState::DemoState(Urho3D::Context *context)
    : GameState(context), mUI(new DemoUI(context)), mCamera(mRegistry),
      mBox(mRegistry) {
  // Let's put some sky in there.
  // Again, if the engine can't find these resources you need to check
  // the "ResourcePrefixPath". These files come with Urho3D.
  auto sky = CreateRenderableEntity("Sky");
  mRegistry.assign<Skybox>(sky, "Models/Box.mdl", "Materials/Skybox.xml");
  // The scale actually does not matter
  mRegistry.assign<Scale>(sky, 500.0f);

  // Let's put a box in there.
  mBox.assign<Name>("Box");
  mBox.assign<Renderable>();
  mBox.assign<StaticModel>("Models/Box.mdl", "Materials/Stone.xml");
  mBox.assign<Position>(0, 2, 15);
  mBox.assign<Direction>();
  mBox.assign<Scale>(3, 3, 3);
  // Rotate the box thingy.
  // A much nicer way of doing this would be with a LogicComponent.
  // With LogicComponents it is easy to control things like movement
  // and animation from some IDE, console or just in game.
  // Alas, it is out of the scope for our simple example.
  mBox.assign<AngularVelocity>(10, 20, 0);

  // Create 400 boxes in a grid.
  for (int x = -30; x < 30; x += 3) {
    for (int z = 0; z < 60; z += 3) {
      auto box = CreateRenderableEntity("Box");
      mRegistry.assign<Position>(box, x, -3, z);
      mRegistry.assign<Scale>(box, 2, 2, 2);
      mRegistry.assign<StaticModel>(box, "Models/Box.mdl", "Materials/Stone.xml");
    }
  }

  // We need a camera from which the viewport can render.
  mCamera.assign<Name>("Camera");
  mCamera.assign<Renderable>();
  mCamera.assign<Camera>().farClip = 2000.0f;
  mCamera.assign<Position>();
  mCamera.assign<Velocity>();
  mCamera.assign<Direction>();
  mCamera.assign<SoundListener>(mCamera.entity());

  // Create a red directional light (sun)
  {
    auto light = CreateRenderableEntity("RedDirectionalLight");
    auto &direction = mRegistry.assign<Direction>(light);
    direction.SetDirection(Urho3D::Vector3::FORWARD);
    direction.Yaw(50);
    direction.Pitch(10);
    auto l = Light{};
    l.type = Urho3D::LIGHT_DIRECTIONAL;
    l.brightness = 1.6;
    l.color = Urho3D::Color(1.0, .6, 0.3, 1);
    l.castShadows = true;
    mRegistry.assign<Light>(light, l);
  }
  // Create a blue point light
  {
    auto light = CreateRenderableEntity("BluePointLight");
    mRegistry.assign<Position>(light, -10, 2, 5);
    auto l = Light{};
    l.type = Urho3D::LIGHT_POINT;
    l.range = 25;
    l.brightness = 1.7;
    l.color = Urho3D::Color(0.5, .5, 1.0, 1);
    l.castShadows = true;
    mRegistry.assign<Light>(light, l);
  }
  // add a green spot light to the camera node
  {
    auto light = CreateRenderableEntity("GreenSpotLight", mCamera.entity());
    mRegistry.assign<Direction>(light).Pitch(15); // point slightly downwards
    auto l = Light{};
    l.type = Urho3D::LIGHT_SPOT;
    l.range = 20;
    l.color = Urho3D::Color(.6, 1, .6, 1.0);
    l.brightness = 2.8;
    l.fov = 25;
    mRegistry.assign<Light>(light, l);
  }
  SetBackgroundMusic("Music/ibi - Some Sand.ogg");

  SubscribeToUpdateEvents();
  SubscribeToKeyDownEvents();
}

void DemoState::OnKeyDown(KeyDownData &data) {
  using namespace Urho3D;
  using namespace KeyDown;
  int key = data.GetKey();
  if (key == KEY_ESCAPE) {
    SendEvent(E_SIGNAL_TERMINATE);
  }

  if (key == KEY_TAB) {
    // toggle mouse cursor when pressing tab
    GetSubsystem<Input>()->SetMouseVisible(
        !GetSubsystem<Input>()->IsMouseVisible());
  }
}

void DemoState::OnUpdate(UpdateEventData &data) {
  // Movement speed as world units per second
  float MOVE_SPEED = 10.0f;
  // Mouse sensitivity as degrees per pixel
  const float MOUSE_SENSITIVITY = 0.1f;

  auto input = GetSubsystem<Urho3D::Input>();
  if (input->GetKeyDown(Urho3D::KEY_SHIFT)) {
    MOVE_SPEED *= 10;
  }

  if (input->GetKeyPress(Urho3D::KEY_SPACE)) {
    auto explosion = ::Sound("Sounds/BigExplosion.wav");
    PlaySound("BigExplosion", explosion, mBox.entity());
  }

  Urho3D::Vector3 direction = Urho3D::Vector3::ZERO;
  if (input->GetKeyDown(Urho3D::KEY_W)) {
    direction += Urho3D::Vector3::FORWARD * MOVE_SPEED;
  }
  if (input->GetKeyDown(Urho3D::KEY_S)) {
    direction += Urho3D::Vector3::BACK * MOVE_SPEED;
  }
  if (input->GetKeyDown(Urho3D::KEY_A)) {
    direction += Urho3D::Vector3::LEFT * MOVE_SPEED;
  }
  if (input->GetKeyDown(Urho3D::KEY_D)) {
    direction += Urho3D::Vector3::RIGHT * MOVE_SPEED;
  }
  mCamera.get<Velocity>().value =
      mCamera.get<Direction>().value * direction;

  if (!GetSubsystem<Urho3D::Input>()->IsMouseVisible()) {
    // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp
    // the pitch between -90 and 90 degrees
    Urho3D::IntVector2 mouseMove = input->GetMouseMove();
    static float yaw_ = 0;
    static float pitch_ = 0;
    yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
    pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
    pitch_ = Urho3D::Clamp(pitch_, -90.0f, 90.0f);
    // Reset rotation and set yaw and pitch again
    auto &direction = mCamera.get<Direction>();
    direction.SetDirection(Urho3D::Vector3::FORWARD);
    direction.Yaw(yaw_);
    direction.Pitch(pitch_);
  }
}
