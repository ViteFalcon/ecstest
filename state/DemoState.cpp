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
#include "../events/GameEvents.h"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Input/Input.h>

DemoState::DemoState(Urho3D::Context *context)
    : GameState(context), mUI(new DemoUI(context)) {
  mScene->CreateComponent<Urho3D::Octree>();
  mScene->CreateComponent<Urho3D::DebugRenderer>();

  // Let's put some sky in there.
  // Again, if the engine can't find these resources you need to check
  // the "ResourcePrefixPath". These files come with Urho3D.
  auto skyNode = mScene->CreateChild("Sky");
  skyNode->SetScale(500.0f); // The scale actually does not matter
  auto skybox = skyNode->CreateComponent<Urho3D::Skybox>();
  skybox->SetModel(mResourceCache.GetResource<Urho3D::Model>("Models/Box.mdl"));
  skybox->SetMaterial(
      mResourceCache.GetResource<Urho3D::Material>("Materials/Skybox.xml"));

  // Let's put a box in there.
  mBoxNode = mScene->CreateChild("Box");
  mBoxNode->SetPosition(Urho3D::Vector3(0, 2, 15));
  mBoxNode->SetScale(Urho3D::Vector3(3, 3, 3));
  auto boxObject = mBoxNode->CreateComponent<Urho3D::StaticModel>();
  boxObject->SetModel(
      mResourceCache.GetResource<Urho3D::Model>("Models/Box.mdl"));
  boxObject->SetMaterial(
      mResourceCache.GetResource<Urho3D::Material>("Materials/Stone.xml"));
  boxObject->SetCastShadows(true);

  // Create 400 boxes in a grid.
  for (int x = -30; x < 30; x += 3)
    for (int z = 0; z < 60; z += 3) {
      Urho3D::Node *boxNode_ = mScene->CreateChild("Box");
      boxNode_->SetPosition(Urho3D::Vector3(x, -3, z));
      boxNode_->SetScale(Urho3D::Vector3(2, 2, 2));
      auto boxObject = boxNode_->CreateComponent<Urho3D::StaticModel>();
      boxObject->SetModel(
          mResourceCache.GetResource<Urho3D::Model>("Models/Box.mdl"));
      boxObject->SetMaterial(
          mResourceCache.GetResource<Urho3D::Material>("Materials/Stone.xml"));
      boxObject->SetCastShadows(true);
    }

  // We need a camera from which the viewport can render.
  mCameraNode = mScene->CreateChild("Camera");
  auto camera = mCameraNode->CreateComponent<Urho3D::Camera>();
  camera->SetFarClip(2000);

  // Create a red directional light (sun)
  {
    auto lightNode = mScene->CreateChild();
    lightNode->SetDirection(Urho3D::Vector3::FORWARD);
    lightNode->Yaw(50);   // horizontal
    lightNode->Pitch(10); // vertical
    auto light = lightNode->CreateComponent<Urho3D::Light>();
    light->SetLightType(Urho3D::LIGHT_DIRECTIONAL);
    light->SetBrightness(1.6);
    light->SetColor(Urho3D::Color(1.0, .6, 0.3, 1));
    light->SetCastShadows(true);
  }
  // Create a blue point light
  {
    auto lightNode = mScene->CreateChild("Light");
    lightNode->SetPosition(Urho3D::Vector3(-10, 2, 5));
    auto light = lightNode->CreateComponent<Urho3D::Light>();
    light->SetLightType(Urho3D::LIGHT_POINT);
    light->SetRange(25);
    light->SetBrightness(1.7);
    light->SetColor(Urho3D::Color(0.5, .5, 1.0, 1));
    light->SetCastShadows(true);
  }
  // add a green spot light to the camera node
  {
    auto node_light = mCameraNode->CreateChild();
    auto light = node_light->CreateComponent<Urho3D::Light>();
    node_light->Pitch(15); // point slightly downwards
    light->SetLightType(Urho3D::LIGHT_SPOT);
    light->SetRange(20);
    light->SetColor(Urho3D::Color(.6, 1, .6, 1.0));
    light->SetBrightness(2.8);
    light->SetFov(25);
  }

  // Now we setup the viewport. Of course, you can have more than one!
  auto renderer = GetSubsystem<Urho3D::Renderer>();
  Urho3D::SharedPtr<Urho3D::Viewport> viewport(
      new Urho3D::Viewport(context_, mScene, camera));
  renderer->SetViewport(0, viewport);

  SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER(DemoState, HandleKeyDown));
  SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(DemoState, HandleUpdate));
}

void DemoState::HandleKeyDown(Urho3D::StringHash eventType,
                              Urho3D::VariantMap &eventData) {
  using namespace Urho3D;
  using namespace KeyDown;
  int key = eventData[P_KEY].GetInt();
  if (key == KEY_ESCAPE) {
    SendEvent(E_SIGNAL_TERMINATE);
  }

  if (key == KEY_TAB) // toggle mouse cursor when pressing tab
  {
    GetSubsystem<Input>()->SetMouseVisible(
        !GetSubsystem<Input>()->IsMouseVisible());
  }
}

void DemoState::HandleUpdate(Urho3D::StringHash eventType,
                             Urho3D::VariantMap &eventData) {
  float timeStep = eventData[Urho3D::Update::P_TIMESTEP].GetFloat();
  // Movement speed as world units per second
  float MOVE_SPEED = 10.0f;
  // Mouse sensitivity as degrees per pixel
  const float MOUSE_SENSITIVITY = 0.1f;

  // Rotate the box thingy.
  // A much nicer way of doing this would be with a LogicComponent.
  // With LogicComponents it is easy to control things like movement
  // and animation from some IDE, console or just in game.
  // Alas, it is out of the scope for our simple example.
  mBoxNode->Rotate(Urho3D::Quaternion(8 * timeStep, 16 * timeStep, 0));

  auto input = GetSubsystem<Urho3D::Input>();
  if (input->GetKeyDown(Urho3D::KEY_SHIFT))
    MOVE_SPEED *= 10;
  if (input->GetKeyDown(Urho3D::KEY_W)) {
    mCameraNode->Translate(Urho3D::Vector3(0, 0, 1) * MOVE_SPEED * timeStep);
  }
  if (input->GetKeyDown(Urho3D::KEY_S)) {
    mCameraNode->Translate(Urho3D::Vector3(0, 0, -1) * MOVE_SPEED * timeStep);
  }
  if (input->GetKeyDown(Urho3D::KEY_A)) {
    mCameraNode->Translate(Urho3D::Vector3(-1, 0, 0) * MOVE_SPEED * timeStep);
  }
  if (input->GetKeyDown(Urho3D::KEY_D)) {
    mCameraNode->Translate(Urho3D::Vector3(1, 0, 0) * MOVE_SPEED * timeStep);
  }

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
    mCameraNode->SetDirection(Urho3D::Vector3::FORWARD);
    mCameraNode->Yaw(yaw_);
    mCameraNode->Pitch(pitch_);
  }
}
