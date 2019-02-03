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

#include "UrhoSystem.h"

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

UrhoSystem::UrhoSystem(Urho3D::Context *context,
                       Urho3D::EntityRegistry &registry,
                       Urho3D::SharedPtr<Urho3D::Scene> scene)
    : Urho3D::EntitySystem(context, registry),
      mRenderer(*context->GetSubsystem<Urho3D::Renderer>()),
      mResources(*context->GetSubsystem<Urho3D::ResourceCache>()),
      mAudio(*context->GetSubsystem<Urho3D::Audio>()), mScene(scene),
      mNodes(*scene, registry), mLights(*scene, registry, mNodes),
      mStaticModels(*scene, registry, mNodes, mResources),
      mCameras(*scene, registry, mNodes, context, mRenderer),
      mSoundListeners(*scene, registry, mNodes, mAudio),
      mBackgroundInstances(*scene, registry, mResources),
      mSounds(*scene, registry, mNodes, mResources),
      mSkyboxes(*scene, registry, mNodes, mResources) {
}

void UrhoSystem::Update(UpdateEventData &data, Urho3D::EntityRegistry &entities) {
  auto renderables = entities.view<Renderable>();
  for (auto entity : renderables) {
    mCameras.Sync(entity, entities);
    mNodes.Sync(entity, entities);
    mStaticModels.Sync(entity, entities);
    mLights.Sync(entity, entities);
    mBackgroundInstances.Sync(entity, entities);
    mSounds.Sync(entity, entities);
    mSoundListeners.Sync(entity, entities);
    mSounds.Sync(entity, entities);
    mSkyboxes.Sync(entity, entities);
  };
}
