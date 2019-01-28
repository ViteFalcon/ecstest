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

#ifndef NINPOTEST_RENDERSYSTEM_H
#define NINPOTEST_RENDERSYSTEM_H

#include <entityx/System.h>

#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

#include "../components/Camera.h"
#include "../components/Light.h"
#include "../components/Material.h"
#include "../components/Renderable.h"
#include "../components/StaticModel.h"

#include "providers/scene/CameraInstances.h"
#include "providers/scene/LightInstances.h"
#include "providers/scene/NodeInstances.h"
#include "providers/scene/StaticModelInstances.h"

class RenderSystem : public entityx::System<RenderSystem>,
                     public entityx::Receiver<RenderSystem> {
public:
  RenderSystem(Urho3D::Context *context,
               Urho3D::SharedPtr<Urho3D::Scene> scene);

  void configure(entityx::EventManager &eventManager) override;

  void update(entityx::EntityManager &entities, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

  void receive(const entityx::ComponentRemovedEvent<Renderable> &event);

  void receive(const entityx::ComponentRemovedEvent<StaticModel> &event);

  void receive(const entityx::ComponentRemovedEvent<Light> &event);

  void receive(const entityx::ComponentRemovedEvent<Camera> &event);

  void receive(const entityx::EntityDestroyedEvent &event);

private:
  Urho3D::Renderer &mRenderer;
  Urho3D::ResourceCache &mResources;
  Urho3D::SharedPtr<Urho3D::Scene> mScene;
  NodeInstances mNodes;
  LightInstances mLights;
  StaticModelInstances mStaticModels;
  CameraInstances mCameras;
};

#endif // NINPOTEST_RENDERSYSTEM_H
