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

#ifndef NINPOTEST_NODEDEPENDENTINSTANCES_H
#define NINPOTEST_NODEDEPENDENTINSTANCES_H

#include "NodeInstances.h"

template <class DerivedType, typename ComponentType, typename ConcreteType,
          typename InstanceComponentType = InstanceComponent<ConcreteType>>
class NodeComponentInstances
    : public SceneInstances<DerivedType, ComponentType, ConcreteType,
                            InstanceComponentType> {
public:
  NodeComponentInstances(Urho3D::Scene &scene, NodeInstances &nodes,
                         Urho3D::String name)
      : SceneInstances<DerivedType, ComponentType, ConcreteType,
                       InstanceComponentType>(scene, name),
        mNodes(nodes) {}

  virtual bool HasDependencies(entityx::Entity entity) const override {
    auto renderable = entity.component<Renderable>();
    return (bool)renderable;
  }

protected:
  virtual Urho3D::SharedPtr<ConcreteType>
  Create(entityx::Entity entity, const ComponentType &component,
         entityx::EntityManager &entities) override {
    Urho3D::SharedPtr<Urho3D::Node> node;
    if (!mNodes.Get(node, entity, entities)) {
      URHO3D_LOGERRORF("Node for '%s' could not be found!",
                       this->GetName(entity).CString());
      return Urho3D::SharedPtr<ConcreteType>{};
    }

    return CreateNodeComponent(entity, *node, component, entities);
  }

  virtual Urho3D::SharedPtr<ConcreteType>
  CreateNodeComponent(entityx::Entity entity, Urho3D::Node &node,
                      const ComponentType &component,
                      entityx::EntityManager &entities) = 0;

  virtual void DestroyInstance(ConcreteType &value) override {
    auto node = value.GetNode();
    node->RemoveComponent(&value);
  }

private:
  NodeInstances &mNodes;
};

#endif // NINPOTEST_NODEDEPENDENTINSTANCES_H
