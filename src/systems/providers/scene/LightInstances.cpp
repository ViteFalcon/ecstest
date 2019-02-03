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

#include "LightInstances.h"

LightInstances::LightInstances(Urho3D::Scene &scene,
                               Urho3D::EntityRegistry &registry,
                               NodeInstances &nodes)
    : NodeComponentInstances(scene, registry, nodes, "Light") {}

Urho3D::SharedPtr<Urho3D::Light> LightInstances::CreateNodeComponent(
    Urho3D::EntityId entityId, Urho3D::Node &node, const Light &component) {
  return Urho3D::SharedPtr<Urho3D::Light>(
      node.CreateComponent<Urho3D::Light>());
}

void LightInstances::SyncFromData(Urho3D::EntityId entityId,
                                  Urho3D::Light &instance, const Light &data) {
  instance.SetLightType(data.type);
  instance.SetBrightness(data.brightness);
  instance.SetColor(data.color);
  instance.SetRadius(data.radius);
  instance.SetRange(data.range);
  instance.SetFov(data.fov);
  instance.SetLength(data.length);
  instance.SetTemperature(data.temperature);
  instance.SetCastShadows(data.castShadows);
}
