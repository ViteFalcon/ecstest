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
    main.cpp
    components/AngularVelocity.h
    components/Direction.h
    components/Light.h
    components/Material.h
    components/Position.h
    components/Renderable.h
    components/Scale.h
    components/StaticModel.h
    components/Velocity.h
    events/BeginFrameData.cpp
    events/BeginFrameData.h
    events/KeyDownData.cpp
    events/KeyDownData.h
    events/GameEvents.h
    events/UpdateEventData.cpp
    events/UpdateEventData.h
    state/DemoState.cpp
    state/DemoState.h
    state/GameState.cpp
    state/GameState.h
    systems/MovementSystem.cpp
    systems/MovementSystem.h
    systems/RenderSystem.cpp
    systems/RenderSystem.h
    ui/DemoUI.cpp
    ui/DemoUI.h
    ui/GameUI.cpp
    ui/GameUI.h
    ui/StatusOverlay.h
    ui/StatusOverlay.cpp
)
# Setup target with resource copying
setup_main_executable(SOURCE_FILES ${SOURCE_FILES})

find_package(EntityX REQUIRED)

target_include_directories(${TARGET_NAME} PUBLIC ${URHO3D_HOME}/include ${ENTITYX_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} ${ENTITYX_LIBRARY})