# Taken from: https://github.com/DavidHamburg/aur-entt/blob/master/FindEntt.cmake
set(ENTT_PATHS
    ${ENTT_ROOT}
	$ENV{ENTT_ROOT}
	/usr/lib/include/entt
)

find_path(ENTT_INCLUDE_DIR entt/entt.hpp PATH_SUFFIXES include PATHS ${ENTT_PATHS})
set(ENTT_INCLUDE_DIRS ${ENTT_INCLUDE_DIR})
include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(entt DEFAULT_MSG ENTT_INCLUDE_DIR)

add_library(ENTT INTERFACE)
target_include_directories(ENTT INTERFACE $<BUILD_INTERFACE:${ENTT_INCLUDE_DIR}>)

mark_as_advanced(ENTT_INCLUDE_DIR)
