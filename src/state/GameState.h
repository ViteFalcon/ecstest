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

#ifndef NINPOTEST_GAMESCENE_H
#define NINPOTEST_GAMESCENE_H

#include <entt/entt.hpp>

#include <Urho3D/Core/Object.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>

#include "../common/Types.h"

#include "../events/BeginFrameData.h"
#include "../events/KeyDownData.h"
#include "../events/UpdateEventData.h"

#include "../components/Name.h"
#include "../components/Renderable.h"
#include "../components/Sound.h"
#include "../events/SoundFinishedEventData.h"
#include "../systems/EntitySystem.h"
#include "../ui/StatusOverlay.h"

class GameState : public Urho3D::Object {
  URHO3D_OBJECT(GameState, Urho3D::Object)
public:
  GameState(Urho3D::Context *context);
  virtual ~GameState() = default;

protected:
  /**
   * Every frame's life must begin somewhere. Here it is.
   */
  virtual void OnBeginFrame(BeginFrameData &data) {}

  /**
   * Input from keyboard is handled here. I'm assuming that Input, if
   * available, will be handled before E_UPDATE.
   */
  virtual void OnKeyDown(KeyDownData &data) {}

  /**
   * Your non-rendering logic should be handled here.
   * This could be moving objects, checking collisions and reaction, etc.
   */
  virtual void OnUpdate(UpdateEventData &data) {}

  /**
   * Anything in the non-rendering logic that requires a second pass,
   * it might be well suited to be handled here.
   */
  virtual void OnPostUpdate(UpdateEventData &data) {}

  /**
   * If you have any details you want to change before the viewport is
   * rendered, try putting it here.
   * See http://urho3d.github.io/documentation/1.32/_rendering.html
   * for details on how the rendering pipeline is setup.
   */
  virtual void OnRenderUpdate(UpdateEventData &data) {}

  /**
   * After everything is rendered, there might still be things you wish
   * to add to the rendering. At this point you cannot modify the scene,
   * only post rendering is allowed. Good for adding things like debug
   * artifacts on screen or brush up lighting, etc.
   */
  virtual void OnPostRenderUpdate(UpdateEventData &data) {}

  /**
   * All good things must come to an end.
   */
  virtual void OnEndFrame() {}

  /**
   * After a sound finishes its execution, this method gets called with the
   * entity that played the sound along with the event data.
   *
   * @param entity entity that held the sound
   * @param data Urho3D's sound finished event data
   */
  virtual void OnSoundFinished(const Urho3D::EntityId entityId,
                               SoundFinishedEventData &data) {}

  void SubscribeToBeginFrameEvents();

  void SubscribeToKeyDownEvents();

  void SubscribeToUpdateEvents();

  void SubscribeToPostUpdateEvents();

  void SubscribeToRenderUpdateEvents();

  void SubscribeToPostRenderUpdateEvents();

  void SubscribeToEndFrameEvents();

  void SubscribeToAllEvents();

private:
  void HandleBeginFrame(Urho3D::StringHash eventType,
                        Urho3D::VariantMap &eventData);

  void HandleKeyDown(Urho3D::StringHash eventType,
                     Urho3D::VariantMap &eventData);

  void HandleUpdate(Urho3D::StringHash eventType,
                    Urho3D::VariantMap &eventData);

  void HandlePostUpdate(Urho3D::StringHash eventType,
                        Urho3D::VariantMap &eventData);

  void HandleRenderUpdate(Urho3D::StringHash eventType,
                          Urho3D::VariantMap &eventData);

  void HandlePostRenderUpdate(Urho3D::StringHash eventType,
                              Urho3D::VariantMap &eventData);

  void HandleEndFrame(Urho3D::StringHash eventType,
                      Urho3D::VariantMap &eventData);

  void HandleSoundFinished(Urho3D::StringHash eventType,
                           Urho3D::VariantMap &eventData);

protected:
  void AddEntitySystem(Urho3D::EntitySystem *system);

  template <typename Component, typename... Args>
  inline Component &assignComponent(const Urho3D::EntityId entity,
                                    Args &&... args) {
    mRegistry.assign(entity, std::forward<Args>(args)...);
  }

  inline Urho3D::Entity CreateEntity() {
    return Urho3D::Entity{mRegistry};
  }

  inline Urho3D::Entity CreateEntity(const Urho3D::String &name) {
    auto entity = CreateEntity();
    entity.assign<Name>(name);
    return entity;
  }

  inline Urho3D::Entity CreateRenderableEntity() {
    auto entity = CreateEntity();
    entity.assign<Renderable>();
    return entity;
  }

  inline Urho3D::Entity CreateRenderableEntity(const Urho3D::String &name) {
    auto entity = CreateEntity(name);
    entity.assign<Renderable>();
    return entity;
  }

  inline Urho3D::Entity CreateRenderableEntity(const Urho3D::EntityId parentId) {
    auto entity = CreateEntity();
    entity.assign<Renderable>(parentId);
    return entity;
  }

  inline Urho3D::Entity CreateRenderableEntity(const Urho3D::String &name,
                                           const Urho3D::EntityId parentId) {
    auto entity = CreateEntity(name);
    entity.assign<Renderable>(parentId);
    return entity;
  }

  void SetBackgroundMusic(const Urho3D::String &filePath);

  void PlaySound(const Sound &sound);
  void PlaySound(const Urho3D::String &name, const Sound &sound);
  void PlaySound(const Sound &sound, const Urho3D::EntityId parentId);
  void PlaySound(const Urho3D::String &name, const Sound &sound,
                 const Urho3D::EntityId parentId);

protected:
  Urho3D::ResourceCache &mResourceCache;
  Urho3D::SharedPtr<Urho3D::Scene> mScene;
  Urho3D::EntityRegistry mRegistry;
  Urho3D::Entity mBackgroundMusic;

private:
  void PlayEntitySound(Urho3D::Entity &entity, const Sound &sound);
};

#define GAME_STATE(ClassName) URHO3D_OBJECT(ClassName, GameState)

#endif // NINPOTEST_GAMESCENE_H
