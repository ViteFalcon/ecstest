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

#include "CameraInstances.h"

#include "../../../components/Viewport.h"

CameraInstances::CameraInstances(Urho3D::Scene &scene,
                                 Urho3D::EntityRegistry &registry,
                                 NodeInstances &nodes, Urho3D::Context *context,
                                 Urho3D::Renderer &renderer)
    : NodeComponentInstances(scene, registry, nodes, "Camera"),
      mContext(context), mRenderer(renderer) {}

Urho3D::SharedPtr<Urho3D::Camera> CameraInstances::CreateNodeComponent(
    Urho3D::EntityId entityId, Urho3D::Node &node, const Camera &component) {
  auto camera = node.CreateComponent<Urho3D::Camera>();
  int viewportIndex = 0;
  Urho3D::RenderPath *renderPath = nullptr;

  if (mRegistry.has<Viewport>(entityId)) {
    auto &viewportData = mRegistry.get<Viewport>(entityId);
    viewportIndex = viewportData.index;
    renderPath = viewportData.renderPath;
  }

  auto viewport = Urho3D::SharedPtr(
      new Urho3D::Viewport(mContext, &mScene, camera, renderPath));
  mRenderer.SetViewport(viewportIndex, viewport);
  return Urho3D::SharedPtr<Urho3D::Camera>(camera);
}

void CameraInstances::SyncFromData(Urho3D::EntityId entityId,
                                   Urho3D::Camera &instance,
                                   const Camera &data) {
  instance.SetNearClip(data.nearClip);
  instance.SetFarClip(data.farClip);
  instance.SetFov(data.fov);
  instance.SetOrthoSize(data.orthoSize);
  instance.SetAspectRatio(data.aspectRatio);
  instance.SetFillMode(data.fillMode);
  instance.SetZoom(data.zoom);
  instance.SetLodBias(data.lodBias);
  instance.SetViewMask(data.viewMask);
  instance.SetViewOverrideFlags(data.viewOverrideFlags);
  instance.SetOrthographic(data.isOrthographic);
  instance.SetAutoAspectRatio(data.isAutoAspectRatio);
  instance.SetProjectionOffset(data.projectionOffset);
  instance.SetUseReflection(data.useReflection);
  instance.SetClipPlane(data.clipPane);
  instance.SetFlipVertical(data.flipVertical);
}
