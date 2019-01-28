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

#ifndef NINPOTEST_CAMERA_H
#define NINPOTEST_CAMERA_H

#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Math/MathDefs.h>
#include <Urho3D/Math/Plane.h>
#include <Urho3D/Graphics/GraphicsDefs.h>
#include <Urho3D/Graphics/Camera.h>

struct Camera {
  float nearClip = Urho3D::DEFAULT_NEARCLIP;
  float farClip = Urho3D::DEFAULT_FARCLIP;
  float fov = Urho3D::DEFAULT_CAMERA_FOV;
  float orthoSize = Urho3D::DEFAULT_ORTHOSIZE;
  float aspectRatio = 1.0f;
  Urho3D::FillMode fillMode = Urho3D::FILL_SOLID;
  float zoom = 1.0f;
  float lodBias = 1.0f;
  unsigned int viewMask = Urho3D::M_MAX_UNSIGNED;
  unsigned int viewOverrideFlags = Urho3D::VO_NONE;
  bool isOrthographic = false;
  bool isAutoAspectRatio = true;
  Urho3D::Vector2 projectionOffset = Urho3D::Vector2::ZERO;
  bool useReflection = false;
  Urho3D::Plane clipPane = Urho3D::Plane::UP;
  bool flipVertical = false;
};

#endif //NINPOTEST_CAMERA_H
