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

#include "NodeInstances.h"

#include "../../../components/Direction.h"
#include "../../../components/Name.h"
#include "../../../components/Position.h"
#include "../../../components/Scale.h"

NodeInstances::NodeInstances(Urho3D::Scene &scene, Urho3D::EntityRegistry &registry)
    : SceneInstances(scene, registry, "Node") {}

Urho3D::SharedPtr<Urho3D::Node>
NodeInstances::Create(Urho3D::EntityId entityId, const Renderable &component) {
  auto name = GetAssignedName(entityId);
  if (component.IsRoot()) {
    return Urho3D::SharedPtr<Urho3D::Node>(mScene.CreateChild(name));
  }

  if (!mRegistry.valid(component.parentEntityId)) {
    URHO3D_LOGERRORF("Count not find parent entity ID to connect to its node "
                     "when constructing '%s'",
                     GetName(entityId).CString());
    return Urho3D::SharedPtr<Urho3D::Node>{};
  }

  Urho3D::SharedPtr<Urho3D::Node> parentNode;
  Urho3D::SharedPtr<Urho3D::Node> node;

  if (!Get(parentNode, component.parentEntityId, mRegistry)) {
    URHO3D_LOGERRORF("Could not find the parent node (%s) to create a child for '%s'. Defaulting to root scene node",
                     GetName(component.parentEntityId).CString(),
                     GetName(entityId).CString());
    node = Urho3D::SharedPtr<Urho3D::Node>(mScene.CreateChild(name));
  } else {
    node = Urho3D::SharedPtr<Urho3D::Node>(parentNode->CreateChild(name));
  }
  node->SetVar(Renderable::ENTITY_ID_NODE_VAR, Urho3D::Variant{entityId});
  return node;
}

void NodeInstances::SyncFromData(Urho3D::EntityId entityId, Urho3D::Node &instance,
                                 const Renderable &data) {
  if (mRegistry.has<Position>(entityId)) {
    auto value = mRegistry.get<Position>(entityId).value;
    instance.SetPosition(value);
  }

  if (mRegistry.has<Direction>(entityId)) {
    auto value = mRegistry.get<Direction>(entityId).value;
    instance.SetRotation(value);
  }

  if (mRegistry.has<Scale>(entityId)) {
    auto value = mRegistry.get<Scale>(entityId).value;
    instance.SetScale(value);
  }
}

bool NodeInstances::DestroyInstance(Urho3D::Node &instance) {
  instance.GetParent()->RemoveChild(&instance);
  return true;
}
