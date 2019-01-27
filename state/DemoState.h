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

#ifndef NINPOTEST_TESTSCENE_H
#define NINPOTEST_TESTSCENE_H

#include "../ui/DemoUI.h"
#include "GameState.h"

class DemoState : public GameState {
  GAME_STATE(DemoState)
public:
  DemoState(Urho3D::Context *context);

  virtual void HandleUpdate(Urho3D::StringHash eventType,
                            Urho3D::VariantMap &eventData) override;

  virtual void HandleKeyDown(Urho3D::StringHash eventType,
                             Urho3D::VariantMap &eventData) override;

private:
  Urho3D::SharedPtr<DemoUI> mUI;
  Urho3D::SharedPtr<Urho3D::Node> mBoxNode;
  Urho3D::SharedPtr<Urho3D::Node> mCameraNode;
};

#endif // NINPOTEST_TESTSCENE_H
