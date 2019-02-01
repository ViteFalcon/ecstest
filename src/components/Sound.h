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

#ifndef NINPOTEST_SOUND_H
#define NINPOTEST_SOUND_H

#include <cassert>

#include <Urho3D/Container/Str.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Audio/AudioDefs.h>

struct Sound {
  explicit Sound(const Urho3D::String value) : value(value) {
    validate();
  }

  Urho3D::String value;
  float nearDistance = 10.0f;
  float farDistance = 100.0f;
  bool isLooped = false;
  Urho3D::String type = Urho3D::SOUND_EFFECT;
  bool isEnabled = true;
  bool isTemporary = true;
  float gain = 1.0f;

private:
  inline void validate() {
    assert(value.EndsWith(".wav", false) && "The sound media should be a WAV file to avoid performance issues");
  }
};

#endif //NINPOTEST_SOUND_H
