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

#include "SkyboxInstances.h"

#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>

SkyboxInstances::SkyboxInstances(Urho3D::Scene &scene, NodeInstances nodes,
                                 Urho3D::ResourceCache &resources)
    : NodeComponentInstances(scene, nodes, "Skybox"), mResources(resources) {}

Urho3D::SharedPtr<Urho3D::Skybox>
SkyboxInstances::CreateNodeComponent(entityx::Entity entity, Urho3D::Node &node,
                                     const Skybox &component,
                                     entityx::EntityManager &entities) {
  return Urho3D::SharedPtr{node.CreateComponent<Urho3D::Skybox>()};
}

void SkyboxInstances::SyncFromData(entityx::Entity entity,
                                   Urho3D::Skybox &instance,
                                   const Skybox &data) {
  if (!data.model.Empty() && (instance.GetModel() == nullptr ||
                              instance.GetModel()->GetName() != data.model)) {
    URHO3D_LOGDEBUGF("Loading skybox model: %s", data.model.CString());
    auto model = mResources.GetResource<Urho3D::Model>(data.model);
    if (model == nullptr) {
      URHO3D_LOGERRORF("Failed to load skybox model: %s", data.model.CString());
    }
    // NOTE: We are setting the model even if failed to load so that the error
    // is obvious
    instance.SetModel(model);
  }

  if (!data.material.Empty() &&
      (instance.GetMaterial() == nullptr ||
       instance.GetMaterial()->GetName() != data.material)) {
    URHO3D_LOGDEBUGF("Loading skybox material: %s", data.material.CString());
    auto material = mResources.GetResource<Urho3D::Material>(data.material);
    if (material == nullptr) {
      URHO3D_LOGERRORF("Failed to load skybox material: %s",
                       data.model.CString());
    }
    // NOTE: We are setting the material even if failed to load so that the
    // error is obvious
    instance.SetMaterial(material);
  }
}
