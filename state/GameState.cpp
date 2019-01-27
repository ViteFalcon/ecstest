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

#include "GameState.h"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/InputEvents.h>

GameState::GameState(Urho3D::Context *context)
    : Urho3D::Object(context),
      mResourceCache(*GetSubsystem<Urho3D::ResourceCache>()),
      mScene(new Urho3D::Scene(context)) {}

void GameState::SubscribeToBeginFrameEvents() {
  SubscribeToEvent(Urho3D::E_BEGINFRAME,
                   URHO3D_HANDLER(GameState, HandleBeginFrame));
}

void GameState::SubscribeToKeyDownEvents() {
  SubscribeToEvent(Urho3D::E_KEYDOWN, URHO3D_HANDLER(GameState, HandleKeyDown));
}

void GameState::SubscribeToUpdateEvents() {
  SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(GameState, HandleUpdate));
}

void GameState::SubscribeToPostUpdateEvents() {
  SubscribeToEvent(Urho3D::E_POSTUPDATE,
                   URHO3D_HANDLER(GameState, HandlePostUpdate));
}

void GameState::SubscribeToRenderUpdateEvents() {
  SubscribeToEvent(Urho3D::E_RENDERUPDATE,
                   URHO3D_HANDLER(GameState, HandleRenderUpdate));
}

void GameState::SubscribeToPostRenderUpdateEvents() {
  SubscribeToEvent(Urho3D::E_POSTRENDERUPDATE,
                   URHO3D_HANDLER(GameState, HandlePostRenderUpdate));
}

void GameState::SubscribeToEndFrameEvents() {
  SubscribeToEvent(Urho3D::E_ENDFRAME,
                   URHO3D_HANDLER(GameState, HandleEndFrame));
}

void GameState::SubscribeToAllEvents() {
  SubscribeToBeginFrameEvents();
  SubscribeToKeyDownEvents();
  SubscribeToUpdateEvents();
  SubscribeToPostUpdateEvents();
  SubscribeToRenderUpdateEvents();
  SubscribeToPostRenderUpdateEvents();
  SubscribeToEndFrameEvents();
}

void GameState::HandleBeginFrame(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap &eventData) {
  BeginFrameData data{eventData};
  OnBeginFrame(data);
}

void GameState::HandleKeyDown(Urho3D::StringHash eventType,
                              Urho3D::VariantMap &eventData) {
  KeyDownData data{eventData};
  OnKeyDown(data);
}

void GameState::HandleUpdate(Urho3D::StringHash eventType,
                             Urho3D::VariantMap &eventData) {
  UpdateEventData data{eventData};
  OnUpdate(data);
}

void GameState::HandlePostUpdate(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap &eventData) {
  UpdateEventData data{eventData};
  OnPostUpdate(data);
}

void GameState::HandleRenderUpdate(Urho3D::StringHash eventType,
                                   Urho3D::VariantMap &eventData) {
  UpdateEventData data{eventData};
  OnRenderUpdate(data);
}

void GameState::HandlePostRenderUpdate(Urho3D::StringHash eventType,
                                       Urho3D::VariantMap &eventData) {
  UpdateEventData data{eventData};
  OnPostRenderUpdate(data);
}

void GameState::HandleEndFrame(Urho3D::StringHash eventType,
                               Urho3D::VariantMap &eventData) {
  OnEndFrame();
}
