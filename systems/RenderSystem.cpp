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
    : mRenderer(*context->GetSubsystem<Urho3D::Renderer>()),
      mResources(*context->GetSubsystem<Urho3D::ResourceCache>()),
      mScene(scene), mNodes(*scene), mLights(*scene, mNodes),
      mStaticModels(*scene, mNodes, mResources),
      mCameras(*scene, mNodes, context, mRenderer) {}

void RenderSystem::configure(entityx::EventManager &eventManager) {
  eventManager.subscribe<entityx::ComponentRemovedEvent<Renderable>>(*this);
  eventManager.subscribe<entityx::ComponentRemovedEvent<StaticModel>>(*this);
  eventManager.subscribe<entityx::ComponentRemovedEvent<Light>>(*this);
  eventManager.subscribe<entityx::ComponentRemovedEvent<Camera>>(*this);
  eventManager.subscribe<entityx::EntityDestroyedEvent>(*this);
}

void RenderSystem::update(entityx::EntityManager &entities,
                          entityx::EventManager &events,
                          entityx::TimeDelta dt) {
  entities.each<Renderable>(
      [&, this](entityx::Entity entity, Renderable &renderable) {
        mCameras.Sync(entity, entities);
        mNodes.Sync(entity, entities);
        mStaticModels.Sync(entity, entities);
        mLights.Sync(entity, entities);
      });
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<Renderable> &event) {
  auto entity = event.entity;
  mNodes.Destroy(entity);
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<StaticModel> &event) {
  mStaticModels.Destroy(event.entity);
}

void RenderSystem::receive(const entityx::ComponentRemovedEvent<Light> &event) {
  auto entity = event.entity;
  mLights.Destroy(entity);
}

void RenderSystem::receive(
    const entityx::ComponentRemovedEvent<Camera> &event) {
  auto entity = event.entity;
  mCameras.Destroy(entity);
}

void RenderSystem::receive(const entityx::EntityDestroyedEvent &event) {
  auto entity = event.entity;
  auto node = mNodes.GetIfExists(entity);

  if (node) {
    mScene->RemoveChild(node);
    return;
  }
  URHO3D_LOGINFO(
      "Failed to find node for entity. Skipping detaching the node.");
}
