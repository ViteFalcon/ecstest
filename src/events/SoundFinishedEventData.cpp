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

#include "SoundFinishedEventData.h"

#include <Urho3D/Audio/AudioEvents.h>

SoundFinishedEventData::SoundFinishedEventData(Urho3D::VariantMap &data)
    : mData(data) {}

Urho3D::Node *SoundFinishedEventData::GetNode() const {
  return static_cast<Urho3D::Node *>(
      mData[Urho3D::SoundFinished::P_NODE].GetPtr());
}

Urho3D::SoundSource *SoundFinishedEventData::GetSoundSource() const {
  return static_cast<Urho3D::SoundSource *>(
      mData[Urho3D::SoundFinished::P_SOUNDSOURCE].GetPtr());
}

Urho3D::Sound *SoundFinishedEventData::GetSound() const {
  return static_cast<Urho3D::Sound *>(
      mData[Urho3D::SoundFinished::P_SOUND].GetPtr());
}
