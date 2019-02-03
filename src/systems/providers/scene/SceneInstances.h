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
#include "../../../common/Types.h"
#include "../../../components/Name.h"

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <sstream>

template <typename ConcreteType> struct InstanceComponent {
  InstanceComponent(Urho3D::SharedPtr<ConcreteType> value) : value(value) {}

  Urho3D::SharedPtr<ConcreteType> value;
};

template <class DerivedType, typename ComponentType, typename ConcreteType,
          typename InstanceComponentType = InstanceComponent<ConcreteType>>
class SceneInstances {
protected:
  Urho3D::Scene &mScene;
  Urho3D::EntityRegistry &mRegistry;
  Urho3D::String mInstanceName;

public:
  SceneInstances(Urho3D::Scene &scene, Urho3D::EntityRegistry &registry,
                 Urho3D::String instanceName)
      : mScene(scene), mRegistry(registry), mInstanceName(instanceName) {
    auto destructionCallback = registry.destruction<ComponentType>();
    destructionCallback
        .template connect<SceneInstances, &SceneInstances::onComponentRemoved>(
            this);
  }
  virtual ~SceneInstances() = default;

  void onComponentRemoved(Urho3D::EntityRegistry &registry,
                          Urho3D::EntityId entityId) {
    Destroy(entityId);
  }

  virtual bool HasDependencies(Urho3D::EntityId entityId) const { return true; }

  Urho3D::SharedPtr<ConcreteType>
  GetIfExists(Urho3D::EntityId entityId, Urho3D::EntityRegistry &registry) {
    if (registry.has<InstanceComponentType>(entityId)) {
      return registry.get<InstanceComponentType>(entityId).value;
    }
    return Urho3D::SharedPtr<ConcreteType>{};
  }

  bool Get(Urho3D::SharedPtr<ConcreteType> &out, Urho3D::EntityId entityId,
           Urho3D::EntityRegistry &registry) {
    if (!registry.has<ComponentType>(entityId)) {
      out.Reset();
      return false;
    }

    out = GetIfExists(entityId, registry);
    if (out) {
      return true;
    }

    if (!HasDependencies(entityId)) {
      return false;
    }

    auto &component = registry.get<ComponentType>(entityId);
    auto instanceComponent = CreateInstanceComponent(entityId, component);
    if (!instanceComponent.value) {
      URHO3D_LOGERRORF("Failed to create concrete instance of type '%s'",
                       GetName(entityId).CString());
      return false;
    }
    registry.assign<InstanceComponentType>(entityId, instanceComponent);
    out = instanceComponent.value;
    URHO3D_LOGDEBUGF("Loaded '%s' instance", GetName(entityId).CString());
    return true;
  }

  void Sync(Urho3D::EntityId entityId, Urho3D::EntityRegistry &registry) {
    if (!registry.has<ComponentType>(entityId)) {
      return;
    }

    Urho3D::SharedPtr<ConcreteType> instance;
    if (!Get(instance, entityId, registry)) {
      URHO3D_LOGERRORF("Failed to find concrete instance for component '%s'",
                       GetName(entityId).CString());
      return;
    }

    auto &data = registry.get<ComponentType>(entityId);
    SyncFromData(entityId, *instance, data);
  }

  void Destroy(Urho3D::EntityId entityId) {
    Urho3D::SharedPtr<ConcreteType> instance{nullptr};
    if (mRegistry.has<InstanceComponentType>(entityId)) {
      instance = mRegistry.get<InstanceComponentType>(entityId).value;
    }
    if (!instance) {
      URHO3D_LOGINFOF("The concrete instance for '%s' could not be found",
                      GetName(entityId).CString());
      return;
    }
    URHO3D_LOGDEBUGF("Destroying '%s'", GetName(entityId).CString());
    if (!DestroyInstance(*instance)) {
      URHO3D_LOGERRORF("Failed to cleanly clean up entity: '%s'",
                       GetName(entityId).CString());
    }
  }

protected:
  Urho3D::String GetName(Urho3D::EntityId entityId) {
#ifdef URHO3D_LOGGING
    std::stringstream nameBuilder;
    nameBuilder << "[" << GetAssignedName(entityId).CString() << "]("
                << entityId << ")";
    return Urho3D::String{nameBuilder.str().c_str()};
#else
    return Urho3D::String{entity.id().id()};
#endif
  }

  Urho3D::String GetAssignedName(Urho3D::EntityId entityId) {
    if (mRegistry.has<Name>(entityId)) {
      return mRegistry.get<Name>(entityId).value;
    }
    return "<NO-NAME>";
  }

  virtual InstanceComponentType
  CreateInstanceComponent(Urho3D::EntityId entity,
                          const ComponentType &component) {
    auto concreteInstance = Create(entity, component);
    return InstanceComponentType{concreteInstance};
  }

  virtual Urho3D::SharedPtr<ConcreteType>
  Create(Urho3D::EntityId entityId, const ComponentType &component) = 0;

  virtual void SyncFromData(Urho3D::EntityId entityId, ConcreteType &instance,
                            const ComponentType &data) = 0;

  virtual bool DestroyInstance(ConcreteType &instance) = 0;
};

#endif // NINPOTEST_SCENEINSTANCES_H
