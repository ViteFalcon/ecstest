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

#ifndef NINPOTEST_SCENEINSTANCES_H
#define NINPOTEST_SCENEINSTANCES_H

#include "../../../common/Optional.h"
#include "../../../components/Name.h"

#include <entityx/Entity.h>

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

template <typename ConcreteType> struct InstanceComponent {
  InstanceComponent(Urho3D::SharedPtr<ConcreteType> value) : value(value) {}

  Urho3D::SharedPtr<ConcreteType> value;
};

template <typename ComponentType, typename ConcreteType,
          typename InstanceComponentType = InstanceComponent<ConcreteType>>
class SceneInstances {
public:
  SceneInstances(Urho3D::Scene &scene, Urho3D::String instanceName)
      : mScene(scene), mInstanceName(instanceName) {}
  virtual ~SceneInstances() = default;

  virtual bool HasDependencies(entityx::Entity entity) const { return true; }

  Urho3D::SharedPtr<ConcreteType> GetIfExists(entityx::Entity entity) {
    auto instance = entity.component<InstanceComponentType>();
    if (instance) {
      return instance->value;
    }
    return Urho3D::SharedPtr<ConcreteType>{};
  }

  bool Get(Urho3D::SharedPtr<ConcreteType> &out, entityx::Entity entity,
           entityx::EntityManager &entities) {
    auto component = entity.component<ComponentType>();
    if (!component) {
      out.Reset();
      return false;
    }

    out = GetIfExists(entity);
    if (out) {
      return true;
    }

    if (!HasDependencies(entity)) {
      return false;
    }

    auto instanceComponent =
        CreateInstanceComponent(entity, *component, entities);
    if (!instanceComponent.value) {
      URHO3D_LOGERRORF("Failed to create concrete instance of type '%s'",
                       GetName(entity).CString());
      return false;
    }
    entity.assign_from_copy(instanceComponent);
    out = instanceComponent.value;
    URHO3D_LOGDEBUGF("Loaded '%s' instance", GetName(entity).CString());
    return true;
  }

  void Sync(entityx::Entity entity, entityx::EntityManager &entities) {
    auto data = entity.component<ComponentType>();
    if (!data) {
      return;
    }

    Urho3D::SharedPtr<ConcreteType> instance;
    if (!Get(instance, entity, entities)) {
      URHO3D_LOGERRORF("Failed to find concrete instance for component '%s'",
                       GetName(entity).CString());
      return;
    }

    SyncFromData(entity, *instance, *data);
  }

  void Destroy(entityx::Entity entity) {
    auto instance = entity.component<InstanceComponentType>();
    if (!instance || !instance->value) {
      URHO3D_LOGINFOF("The concrete instance for '%s' could not be found",
                      GetName(entity).CString());
      return;
    }
    URHO3D_LOGINFOF("Destroying '%s'", GetName(entity).CString());
    DestroyInstance(*(instance->value));
  }

protected:
  Urho3D::String GetName(entityx::Entity entity) {
    auto name = entity.component<Name>();
    if (name) {
      return mInstanceName + "[" + name->value + "]";
    }
    return mInstanceName + "[NO-NAME]";
  }

  virtual InstanceComponentType
  CreateInstanceComponent(entityx::Entity entity,
                          const ComponentType &component,
                          entityx::EntityManager &entities) {
    auto concreteInstance = Create(entity, component, entities);
    return InstanceComponentType{concreteInstance};
  }

  virtual Urho3D::SharedPtr<ConcreteType>
  Create(entityx::Entity entity, const ComponentType &component,
         entityx::EntityManager &entities) = 0;

  virtual void SyncFromData(entityx::Entity entity, ConcreteType &instance,
                            const ComponentType &data) = 0;

  virtual void DestroyInstance(ConcreteType &instance) = 0;

protected:
  Urho3D::Scene &mScene;
  Urho3D::String mInstanceName;
};

#endif // NINPOTEST_SCENEINSTANCES_H
