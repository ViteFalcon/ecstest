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

#ifndef NINPOTEST_SOUNDLISTENERINSTANCES_H
#define NINPOTEST_SOUNDLISTENERINSTANCES_H

#include "NodeComponentInstances.h"

#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/SoundListener.h>

#include "../../../components/SoundListener.h"

class SoundListenerInstances
    : public NodeComponentInstances<SoundListenerInstances, SoundListener,
                                    Urho3D::SoundListener> {
public:
  SoundListenerInstances(Urho3D::Scene &scene, Urho3D::EntityRegistry &registry,
                         NodeInstances nodes, Urho3D::Audio &audio);

private:
  virtual Urho3D::SharedPtr<Urho3D::SoundListener>
  CreateNodeComponent(Urho3D::EntityId entityId, Urho3D::Node &node,
                      const SoundListener &component) override;

  virtual void SyncFromData(Urho3D::EntityId entityId,
                            Urho3D::SoundListener &instance,
                            const SoundListener &data) override;

  virtual bool DestroyInstance(Urho3D::SoundListener &value) override;

  Urho3D::Audio &mAudio;
  Urho3D::EntityId mCurrentListener;
};

#endif // NINPOTEST_SOUNDLISTENERINSTANCES_H
