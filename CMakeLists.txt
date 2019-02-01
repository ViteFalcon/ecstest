cmake_minimum_required(VERSION 3.13)
project(NinpoTest)

set(CMAKE_CXX_STANDARD 17)

set(URHO3D_HOME /Users/ullatil/Projects/cpp/libs/Urho3D)
# Define target name
set(TARGET_NAME NinpoTest)

# Set CMake minimum version and CMake policy required by Urho3D-CMake-common module
if (WIN32)
  cmake_minimum_required(VERSION 3.2.3)      # Going forward all platforms will use this as minimum version
else ()
  cmake_minimum_required(VERSION 2.8.6)
endif ()
if (COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy(SET CMP0022 NEW)
  endif ()
  if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
    cmake_policy(SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy(SET CMP0042 NEW)
  endif ()
endif ()
# Set CMake modules search path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
# Include Urho3D Cmake common module
include(UrhoCommon)
# Define source files
set(
    SOURCE_FILES
    src/main.cpp
    src/common/None.cpp
    src/common/None.h
    src/common/Optional.h
    src/common/Storage.h
    src/components/AngularVelocity.h
    src/components/BackgroundMusic.h
    src/components/Camera.h
    src/components/Direction.cpp
    src/components/Direction.h
    src/components/Light.h
    src/components/Material.h
    src/components/Name.h
    src/components/Position.h
    src/components/Renderable.cpp
    src/components/Renderable.h
    src/components/Scale.h
    src/components/Sound.h
    src/components/SoundListener.h
    src/components/StaticModel.h
    src/components/Velocity.h
    src/components/Viewport.h
    src/events/BeginFrameData.cpp
    src/events/BeginFrameData.h
    src/events/KeyDownData.cpp
    src/events/KeyDownData.h
    src/events/GameEvents.h
    src/events/UpdateEventData.cpp
    src/events/UpdateEventData.h
    src/state/DemoState.cpp
    src/state/DemoState.h
    src/state/GameState.cpp
    src/state/GameState.h
    src/systems/providers/scene/BackgroundMusicInstances.cpp
    src/systems/providers/scene/BackgroundMusicInstances.h
    src/systems/providers/scene/CameraInstances.cpp
    src/systems/providers/scene/CameraInstances.h
    src/systems/providers/scene/LightInstances.cpp
    src/systems/providers/scene/LightInstances.h
    src/systems/providers/scene/NodeComponentInstances.h
    src/systems/providers/scene/NodeInstances.cpp
    src/systems/providers/scene/NodeInstances.h
    src/systems/providers/scene/SceneInstances.h
    src/systems/providers/scene/SkyboxInstances.cpp
    src/systems/providers/scene/SkyboxInstances.h
    src/systems/providers/scene/SoundInstances.cpp
    src/systems/providers/scene/SoundInstances.h
    src/systems/providers/scene/SoundListenerInstances.cpp
    src/systems/providers/scene/SoundListenerInstances.h
    src/systems/providers/scene/StaticModelInstances.cpp
    src/systems/providers/scene/StaticModelInstances.h
    src/systems/MovementSystem.cpp
    src/systems/MovementSystem.h
    src/systems/UrhoSystem.cpp
    src/systems/UrhoSystem.h
    src/ui/DemoUI.cpp
    src/ui/DemoUI.h
    src/ui/GameUI.cpp
    src/ui/GameUI.h
    src/ui/StatusOverlay.h
    src/ui/StatusOverlay.cpp
)
# Setup target with resource copying
setup_main_executable(SOURCE_FILES ${SOURCE_FILES})

find_package(EntityX REQUIRED)

target_include_directories(${TARGET_NAME} PUBLIC ${URHO3D_HOME}/include ${ENTITYX_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} ${ENTITYX_LIBRARY})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/)