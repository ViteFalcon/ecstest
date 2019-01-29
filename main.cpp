#include <sstream>
#include <string>

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/UI/UI.h>

#include "events/GameEvents.h"
#include "state/DemoState.h"
#include "ui/StatusOverlay.h"

using namespace Urho3D;
/**
 * Using the convenient Application API we don't have
 * to worry about initializing the engine or writing a main.
 * You can probably mess around with initializing the engine
 * and running a main manually, but this is convenient and portable.
 */
class MyApp : public Application {
public:
  SharedPtr<StatusOverlay> status_;
  SharedPtr<DemoState> state_;

  /**
   * This happens before the engine has been initialized
   * so it's usually minimal code setting defaults for
   * whatever instance variables you have.
   * You can also do this in the Setup method.
   */
  MyApp(Context *context) : Application(context) {}

  /**
   * This method is called before the engine has been initialized.
   * Thusly, we can setup the engine parameters before anything else
   * of engine importance happens (such as windows, search paths,
   * resolution and other things that might be user configurable).
   */
  virtual void Setup() {
    // These parameters should be self-explanatory.
    // See http://urho3d.github.io/documentation/1.5/_main_loop.html
    // for a more complete list.
    engineParameters_[Urho3D::EP_FULL_SCREEN] = false;
    engineParameters_[Urho3D::EP_WINDOW_WIDTH] = 1280;
    engineParameters_[Urho3D::EP_WINDOW_HEIGHT] = 720;
    engineParameters_[Urho3D::EP_WINDOW_RESIZABLE] = true;
    engineParameters_[Urho3D::EP_SOUND] = true;
  }

  /**
   * This method is called after the engine has been initialized.
   * This is where you set up your actual content, such as scenes,
   * models, controls and what not. Basically, anything that needs
   * the engine initialized and ready goes in here.
   */
  virtual void Start() {
    // We will be needing to load resources.
    // All the resources used in this example comes with Urho3D.
    // If the engine can't find them, check the ResourcePrefixPath (see
    // http://urho3d.github.io/documentation/1.5/_main_loop.html).
    ResourceCache *cache = GetSubsystem<ResourceCache>();

    // Let's use the default style that comes with Urho3D.
    GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(
        cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

    // Subscribe to terminate signals
    SubscribeToEvent(E_SIGNAL_TERMINATE,
                     URHO3D_HANDLER(MyApp, HandleClosePressed));
    // Let's create some text to display.
    status_ = new StatusOverlay(context_);
    state_ = new DemoState(context_);
  }

  /**
   * Good place to get rid of any system resources that requires the
   * engine still initialized. You could do the rest in the destructor,
   * but there's no need, this method will get called when the engine stops,
   * for whatever reason (short of a segfault).
   */
  virtual void Stop() {
    state_.Reset();
    status_.Reset();
  }

  /**
   * You can get these events from when ever the user interacts with the UI.
   */
  void HandleClosePressed(StringHash eventType, VariantMap &eventData) {
    engine_->Exit();
  }
};

/**
 * This macro is expanded to (roughly, depending on OS) this:
 *
 * > int RunApplication()
 * > {
 * > Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
 * > Urho3D::SharedPtr<className> application(new className(context));
 * > return application->Run();
 * > }
 * >
 * > int main(int argc, char** argv)
 * > {
 * > Urho3D::ParseArguments(argc, argv);
 * > return function;
 * > }
 */
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
