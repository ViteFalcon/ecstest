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

NodeInstances::NodeInstances(Urho3D::Scene &scene)
    : SceneInstances(scene, "Node") {}

Urho3D::SharedPtr<Urho3D::Node>
NodeInstances::Create(entityx::Entity entity, const Renderable &component,
                      entityx::EntityManager &entities) {
  auto name = GetAssignedName(entity);
  if (component.IsRoot()) {
    return Urho3D::SharedPtr<Urho3D::Node>(mScene.CreateChild(name));
  }
  auto parentEntity = entities.get(component.parentEntityId);
  if (!parentEntity) {
    URHO3D_LOGERRORF("Count not find parent entity ID to connect to its node "
                     "when constructing '%s'",
                     GetName(entity).CString());
    return Urho3D::SharedPtr<Urho3D::Node>{};
  }
  Urho3D::SharedPtr<Urho3D::Node> parentNode;
  if (!Get(parentNode, parentEntity, entities)) {
    URHO3D_LOGERRORF("Could not find the parent node (%s) create a child for "
                     "'%s'. Defaulting to root scene node",
                     GetName(parentEntity).CString(),
                     GetName(entity).CString());
    return Urho3D::SharedPtr<Urho3D::Node>(mScene.CreateChild(name));
  }
  return Urho3D::SharedPtr<Urho3D::Node>(parentNode->CreateChild(name));
}

void NodeInstances::SyncFromData(entityx::Entity entity, Urho3D::Node &instance,
                                 const Renderable &data) {
  auto pos = entity.component<Position>();
  if (pos) {
    instance.SetPosition(pos->value);
  }
  auto dir = entity.component<Direction>();
  if (dir) {
    instance.SetRotation(dir->value);
  }
  auto scale = entity.component<Scale>();
  if (scale) {
    instance.SetScale(scale->value);
  }
}

void NodeInstances::DestroyInstance(Urho3D::Node &instance) {
  instance.GetParent()->RemoveChild(&instance);
}
