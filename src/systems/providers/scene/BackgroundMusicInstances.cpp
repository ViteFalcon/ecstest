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

#include "BackgroundMusicInstances.h"

#include <Urho3D/Audio/SoundSource.h>

BackgroundMusicInstances::BackgroundMusicInstances(
    Urho3D::Scene &scene, Urho3D::EntityRegistry &registry,
    Urho3D::ResourceCache &resources)
    : SceneInstances<BackgroundMusicInstances, BackgroundMusic,
                     Urho3D::SoundSource>(scene, registry, "BackgroundMusic"),
      mResources(resources) {}

Urho3D::SharedPtr<Urho3D::SoundSource>
BackgroundMusicInstances::Create(Urho3D::EntityId entityId,
                                 const BackgroundMusic &component) {
  auto soundSource = mScene.CreateComponent<Urho3D::SoundSource>();
  if (PlayMusic(*soundSource, component.value)) {
    return Urho3D::SharedPtr(soundSource);
  }
  // Clean up
  mScene.RemoveComponent(soundSource);
  return Urho3D::SharedPtr<Urho3D::SoundSource>{};
}

void BackgroundMusicInstances::SyncFromData(Urho3D::EntityId entityId,
                                            Urho3D::SoundSource &instance,
                                            const BackgroundMusic &data) {
  if (mSound->GetName() == data.value) {
    return;
  }
  instance.Stop();
  PlayMusic(instance, data.value);
}

bool BackgroundMusicInstances::DestroyInstance(Urho3D::SoundSource &instance) {
  instance.Stop();
  mSound.Reset();
  mScene.RemoveComponent(&instance);
  return true;
}

bool BackgroundMusicInstances::PlayMusic(Urho3D::SoundSource &soundSource,
                                         const Urho3D::String file) {
  mSound = mResources.GetResource<Urho3D::Sound>(file);
  if (!mSound) {
    URHO3D_LOGERRORF("Failed to load background music from file: %s",
                     file.CString());
    return false;
  }
  mSound->SetName(file);
  mSound->SetLooped(true);
  soundSource.SetSoundType(Urho3D::SOUND_MUSIC);
  soundSource.Play(mSound);
  return true;
}
