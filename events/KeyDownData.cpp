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

#include "KeyDownData.h"

#include <Urho3D/Input/InputEvents.h>

KeyDownData::KeyDownData(Urho3D::VariantMap &data) : mData(data) {}

int KeyDownData::GetKey() const {
  return mData[Urho3D::KeyDown::P_KEY].GetInt();
}

int KeyDownData::GetScanCode() const {
  return mData[Urho3D::KeyDown::P_SCANCODE].GetInt();
}

int KeyDownData::GetButtons() const {
  return mData[Urho3D::KeyDown::P_BUTTONS].GetInt();
}

int KeyDownData::GetQualifiers() const {
  return mData[Urho3D::KeyDown::P_QUALIFIERS].GetInt();
}

bool KeyDownData::IsRepeat() const {
  return mData[Urho3D::KeyDown::P_REPEAT].GetBool();
}
