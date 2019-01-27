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

#include "RenderSystem.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Node.h>

#include "../components/Direction.h"
#include "../components/Position.h"
#include "../components/Renderable.h"
#include "../components/Scale.h"

RenderSystem::RenderSystem(Urho3D::Context *context,
                           Urho3D::SharedPtr<Urho3D::Scene> scene)
    : mResources(*context->GetSubsystem<Urho3D::ResourceCache>()),
      mScene(scene) {}

void RenderSystem::configure(entityx::EventManager &eventManager) {
  eventManager.subscribe<entityx::ComponentAddedEvent<Renderable>>(*this);
  eventManager.subscribe<entityx::ComponentRemovedEvent<Renderable>>(*this);
  eventManager.subscribe<entityx::ComponentAddedEvent<StaticModel>>(*this);
  eventManager.subscribe<entityx::ComponentRemovedEvent<StaticModel>>(*this);
  eventManager.subscribe<entityx::ComponentAddedEvent<Light>>(*this);
  eventManager.subscribe<entityx::ComponentRemovedEvent<Light>>(*this);
  eventManager.subscribe<entityx::EntityDestroyedEvent>(*this);
}

void RenderSystem::update(entityx::EntityManager &entities,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  entities.each<Renderable>(
      [&, this](entityx::Entity entity, Renderable &renderable) {
        auto node = GetEntityNode(entity);
        auto pos = entity.component<Position>();
        if (pos) {
          node->SetPosition(pos->value);
        }
        auto dir = entity.component<Direction>();
        if (dir) {
          node->SetRotation(dir->value);
        }
        auto scale = entity.component<Scale>();
        if (scale) {
          node->SetScale(scale->value);
        }
      });
}

void RenderSystem::receive(
    const entityx::ComponentAddedEvent<Renderable> &event) {
  using namespace Urho3D;
  CreateEntityNode(event.entity, *event.component);
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<Renderable> &event) {
  auto node = GetEntityNode(event.entity);
  if (node == nullptr) {
    URHO3D_LOGERROR("Failed to find node for renderable");
    return;
  }
  mNodes.Erase(event.entity.id().id());
  mScene->RemoveChild(node);
}

void RenderSystem::receive(
    const entityx::ComponentAddedEvent<StaticModel> &event) {
  auto entity = event.entity;
  auto renderable = entity.component<Renderable>();
  if (!renderable) {
    return;
  }
  auto node = GetEntityNode(event.entity);
  if (node == nullptr) {
    URHO3D_LOGERROR("Failed to find node for entity");
    return;
  }
  auto data = event.component;
  auto staticModel = node->CreateComponent<Urho3D::StaticModel>();
  auto model = mResources.GetResource<Urho3D::Model>(data->model);
  staticModel->SetModel(model);
  auto material = mResources.GetResource<Urho3D::Material>(data->material);
  staticModel->SetMaterial(material);
  staticModel->SetCastShadows(data->castShadows);
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<StaticModel> &event) {
  auto entity = event.entity;
  auto renderable = entity.component<Renderable>();
  if (!renderable) {
    return;
  }
  auto node = GetEntityNode(event.entity);
  if (node == nullptr) {
    URHO3D_LOGERROR("Failed to find node for entity");
    return;
  }
  node->RemoveComponent<Urho3D::StaticModel>();
}

void RenderSystem::receive(const entityx::EntityDestroyedEvent &event) {
  auto entity = event.entity;
  auto renderable = entity.component<Renderable>();
  if (!renderable) {
    return;
  }
  auto node = GetEntityNode(event.entity);
  if (node == nullptr) {
    URHO3D_LOGINFO(
        "Failed to find node for entity. Skipping detaching the node.");
    return;
  }
  mScene->RemoveChild(node);
}

void RenderSystem::receive(const entityx::ComponentAddedEvent<Light> &event) {
  auto entity = event.entity;
  auto renderable = entity.component<Renderable>();
  if (!renderable) {
    return;
  }
  auto node = GetEntityNode(entity);
  if (node == nullptr) {
    URHO3D_LOGERROR("Failed to find node for entity");
    return;
  }
  auto data = event.component;
  auto light = node->CreateComponent<Urho3D::Light>();
  light->SetLightType(data->type);
  light->SetBrightness(data->brightness);
  light->SetColor(data->color);
  light->SetRadius(data->radius);
  light->SetRange(data->range);
  light->SetFov(data->fov);
  light->SetLength(data->length);
  light->SetTemperature(data->temperature);
  light->SetCastShadows(data->castShadows);
}

void RenderSystem::receive(const entityx::ComponentRemovedEvent<Light> &event) {
  auto entity = event.entity;
  auto renderable = entity.component<Renderable>();
  if (!renderable) {
    return;
  }
  auto node = GetEntityNode(entity);
  if (node == nullptr) {
    URHO3D_LOGERROR("Failed to find node for entity");
    return;
  }
  node->RemoveComponent<Urho3D::Light>();
}

inline Urho3D::Node *
RenderSystem::CreateEntityNode(const entityx::Entity &entity,
                               const Renderable &renderable) {
  auto node = renderable.parentEntityId == entityx::Entity::INVALID
                  ? mScene->CreateChild(renderable.name)
                  : mNodes[renderable.parentEntityId.id()]->CreateChild(
                        renderable.name);
  if (node == nullptr) {
    URHO3D_LOGERRORF("Failed to find node for entity with ID: {}",
                     renderable.parentEntityId);
    node = mScene->CreateChild(renderable.name);
  }
  mNodes[entity.id().id()] = node;
  return node;
}

inline Urho3D::Node *
RenderSystem::GetEntityNode(const entityx::Entity &entity) {
  return mNodes[entity.id().id()];
}
