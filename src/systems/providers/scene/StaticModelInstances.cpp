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

#include "StaticModelInstances.h"

#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/IO/Log.h>

StaticModelInstances::StaticModelInstances(Urho3D::Scene &scene,
                                           NodeInstances nodes,
                                           Urho3D::ResourceCache &resources)
    : NodeComponentInstances(scene, nodes, "StaticModel"),
      mResources(resources) {}

StaticModelInstance StaticModelInstances::CreateInstanceComponent(
    entityx::Entity entity, const StaticModel &component,
    entityx::EntityManager &entities) {
  Urho3D::SharedPtr<Urho3D::StaticModel> model(
      Create(entity, component, entities));
  if (!model) {
    return StaticModelInstance{};
  }
  StaticModelInstance instance{model};
  LoadModel(instance.cached, *model, component);
  URHO3D_LOGDEBUGF("Loaded model: %s", instance.cached.model.CString());
  LoadMaterial(instance.cached, *model, component);
  URHO3D_LOGDEBUGF("Loaded material: %s", instance.cached.material.CString());
  return instance;
}

Urho3D::SharedPtr<Urho3D::StaticModel> StaticModelInstances::CreateNodeComponent(
    entityx::Entity entity, Urho3D::Node &node, const StaticModel &component,
    entityx::EntityManager &entities) {
  return Urho3D::SharedPtr<Urho3D::StaticModel>(node.CreateComponent<Urho3D::StaticModel>());
}

void StaticModelInstances::SyncFromData(entityx::Entity entity,
                                        Urho3D::StaticModel &instance,
                                        const StaticModel &data) {
  auto model = entity.component<StaticModelInstance>();
  if (model->cached.model != data.model) {
    LoadModel(model->cached, instance, data);
  }

  if (model->cached.material != data.material) {
    LoadMaterial(model->cached, instance, data);
  }
}

void StaticModelInstances::LoadModel(StaticModel &cachedData,
                                     Urho3D::StaticModel &instance,
                                     const StaticModel &data) {
  URHO3D_LOGDEBUGF("Loading model: %s", data.model.CString());
  auto model = mResources.GetResource<Urho3D::Model>(data.model);
  instance.SetModel(model);
  cachedData.model = data.model;
}

void StaticModelInstances::LoadMaterial(StaticModel &cachedData,
                                        Urho3D::StaticModel &instance,
                                        const StaticModel &data) {
  URHO3D_LOGDEBUGF("Loading material: %s", data.material.CString());
  auto material = mResources.GetResource<Urho3D::Material>(data.material);
  instance.SetMaterial(material);
  cachedData.material = data.material;
}
