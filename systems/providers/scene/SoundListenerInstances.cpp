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

#include "SoundListenerInstances.h"

SoundListenerInstances::SoundListenerInstances(Urho3D::Scene &scene,
                                               NodeInstances nodes,
                                               Urho3D::Audio &audio)
    : NodeComponentInstances(scene, nodes, "SoundListener"), mAudio(audio),
      mCurrentListener(entityx::Entity::INVALID) {}

Urho3D::SharedPtr<Urho3D::SoundListener>
SoundListenerInstances::CreateNodeComponent(entityx::Entity entity,
                                            Urho3D::Node &node,
                                            const SoundListener &component,
                                            entityx::EntityManager &entities) {
  auto previousListener = mCurrentListener;
  if (entityx::Entity::INVALID != mCurrentListener) {
    // There can only be a single listener
    entities.get(previousListener).remove<SoundListener>();
  }
  auto listener = node.CreateComponent<Urho3D::SoundListener>();
  mAudio.SetListener(listener);
  mCurrentListener = entity.id();
  return Urho3D::SharedPtr(listener);
}

void SoundListenerInstances::SyncFromData(entityx::Entity entity,
                                          Urho3D::SoundListener &instance,
                                          const SoundListener &data) {
  // Nothing to do
}

void SoundListenerInstances::DestroyInstance(Urho3D::SoundListener &value) {
  mAudio.SetListener(nullptr);
  NodeComponentInstances::DestroyInstance(value);
}
