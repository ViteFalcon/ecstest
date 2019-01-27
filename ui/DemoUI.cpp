/*
------------------------------------------------------------------------------------------------------------------------
Copyright 2019 Vite Falcon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------------------------------
*/

#include "DemoUI.h"
#include "../events/GameEvents.h"

#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>

DemoUI::DemoUI(Urho3D::Context *context):GameUI(context) {
  // Add a button, just as an interactive UI sample.
  Urho3D::Button* button=new Urho3D::Button(context_);
  button->SetName("Button Quit");
  button->SetStyle("Button");
  button->SetSize(32,32);
  button->SetPosition(16,116);

  // Subscribe to button release (following a 'press') events
  SubscribeToEvent(button,Urho3D::E_RELEASED,URHO3D_HANDLER(DemoUI,HandleQuitEvent));

  addElement(button);
}

void DemoUI::HandleQuitEvent(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
  SendEvent(E_SIGNAL_TERMINATE);
}
