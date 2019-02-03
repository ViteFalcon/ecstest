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

#include "SoundInstances.h"

#include <Urho3D/Audio/AudioEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/SceneEvents.h>

SoundInstances::SoundInstances(Urho3D::Scene &scene,
                               Urho3D::EntityRegistry &registry,
                               NodeInstances &nodes,
                               Urho3D::ResourceCache &resources)
    : NodeComponentInstances(scene, registry, nodes, "Sound"),
      mResources(resources) {}

Urho3D::SharedPtr<Urho3D::SoundSource3D> SoundInstances::CreateNodeComponent(
    Urho3D::EntityId entityId, Urho3D::Node &node, const Sound &component) {
  auto sound = mResources.GetResource<Urho3D::Sound>(component.value);
  if (!sound) {
    URHO3D_LOGERRORF("Failed to load sound: %s", component.value.CString());
    return Urho3D::SharedPtr<Urho3D::SoundSource3D>{};
  }

  Urho3D::SharedPtr<Urho3D::SoundSource3D> source(
      node.CreateComponent<Urho3D::SoundSource3D>());
  if (!source) {
    URHO3D_LOGERROR("Failed to create sound source 3D");
    return Urho3D::SharedPtr<Urho3D::SoundSource3D>{};
  }
  mSounds[source->GetID()] = entityId;
  sound->SetLooped(component.isLooped);
  SyncFromData(entityId, *source, component);
  source->Play(sound);

  URHO3D_LOGDEBUGF("Added sound '%s' to entity ID: '%s'",
                   sound->GetName().CString(),
                   this->GetName(entityId).CString());
  return source;
}

void SoundInstances::SyncFromData(Urho3D::EntityId entityId,
                                  Urho3D::SoundSource3D &source,
                                  const Sound &data) {
  source.SetNearDistance(data.nearDistance);
  source.SetFarDistance(data.farDistance);
  source.SetSoundType(data.type);
  source.SetEnabled(data.isEnabled);
  source.SetGain(data.gain);
}

bool SoundInstances::DestroyInstance(Urho3D::SoundSource3D &value) {
  auto itr = mSounds.Find(value.GetID());
  if (itr == mSounds.End()) {
    URHO3D_LOGERRORF("Failed to find entity playing sound: %s",
                     value.GetSound()->GetName().CString());
  } else {
    mSounds.Erase(itr);
  }
  return NodeComponentInstances::DestroyInstance(value);
}
