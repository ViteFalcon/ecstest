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

#ifndef NINPOTEST_LIGHT_H
#define NINPOTEST_LIGHT_H

#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Math/Color.h>

struct Light {
  Urho3D::LightType type;
  /// Brightness multiplier.
  float brightness;
  float range;
  /// Spotlight field of view.
  float fov;
  /// Light temperature.
  float temperature;
  /// Radius of the light source. If above 0 it will turn the light into an area light.  Works only with PBR shaders.
  float radius;
  /// Length of the light source. If above 0 and radius is above 0 it will create a tube light. Works only with PBR shaders.
  float length;
  Urho3D::Color color;
  bool castShadows;
};

#endif //NINPOTEST_LIGHT_H
