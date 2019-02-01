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

#include "StatusOverlay.h"

#include <sstream>
#include <string>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Math/Color.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#define STATUS_OVERLAY_FIRST_LINE                                              \
  "Keys: tab = toggle mouse, AWSD = move camera, Shift = fast mode, Esc = "    \
  "quit, Space = EXPLOSIONS!\n"

StatusOverlay::StatusOverlay(Urho3D::Context *context)
    : GameUI(context), mFrameCount(0), mTime(0.0f) {
  Urho3D::ResourceCache *cache = GetSubsystem<Urho3D::ResourceCache>();
  Urho3D::Text *text = new Urho3D::Text(context);
  // Text will be updated later in the E_UPDATE handler. Keep readin'.
  text->SetText(STATUS_OVERLAY_FIRST_LINE "Wait a bit to see FPS.");
  // If the engine cannot find the font, it comes with Urho3D.
  // Set the environment variables URHO3D_HOME, URHO3D_PREFIX_PATH or
  // change the engine parameter "ResourcePrefixPath" in the Setup method.
  text->SetFont(cache->GetResource<Urho3D::Font>("Fonts/Anonymous Pro.ttf"),
                18);
  text->SetColor(Urho3D::Color(.3, 0, .3));
  text->SetHorizontalAlignment(Urho3D::HA_CENTER);
  text->SetVerticalAlignment(Urho3D::VA_TOP);

  setNamedElement("text", text, true);

  SubscribeToEvent(Urho3D::E_UPDATE,
                   URHO3D_HANDLER(StatusOverlay, HandleUpdate));
}

void StatusOverlay::HandleUpdate(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap &eventData) {
  float timeStep = eventData[Urho3D::Update::P_TIMESTEP].GetFloat();
  mFrameCount++;
  mTime += timeStep;

  if (mTime < 1) {
    return;
  }

  std::string str;
  str.append(STATUS_OVERLAY_FIRST_LINE);
  {
    std::ostringstream ss;
    ss << mFrameCount;
    std::string s(ss.str());
    str.append(s.substr(0, 6));
  }
  str.append(" frames in ");
  {
    std::ostringstream ss;
    ss << mTime;
    std::string s(ss.str());
    str.append(s.substr(0, 6));
  }
  str.append(" seconds = ");
  {
    std::ostringstream ss;
    ss << (float)mFrameCount / mTime;
    std::string s(ss.str());
    str.append(s.substr(0, 6));
  }
  str.append(" fps");
  Urho3D::String s(str.c_str(), str.size());
  getNamedElement<Urho3D::Text>("text")->SetText(s);
  mFrameCount = 0;
  mTime = 0;
}
