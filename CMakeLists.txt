# Set default build type to Debug if not specified (single-config generators)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set debug flags for MSVC and GCC/Clang
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /DDEBUG")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")
endif()

cmake_minimum_required(VERSION 3.16...4.01)
project(the_game)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# -------------------------
# Fetch raylib (has CMake support)
# -------------------------
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# -------------------------
# Fetch hoxml (header-only, raytmx dependency)
# -------------------------
FetchContent_Declare(
  hoxml
  GIT_REPOSITORY https://github.com/luphi/hoxml.git
  GIT_TAG main
)
FetchContent_GetProperties(hoxml)
if(NOT hoxml_POPULATED)
  FetchContent_MakeAvailable(hoxml)
endif()
add_library(hoxml INTERFACE)
target_include_directories(hoxml INTERFACE ${hoxml_SOURCE_DIR})

# -------------------------
# raytmx (header-only, depends on hoxml)
# -------------------------
set(RAYTMX_LOCAL_PATH "${CMAKE_SOURCE_DIR}/external/raytmx")

if(EXISTS ${RAYTMX_LOCAL_PATH}/raytmx.h)
    # Use submodule
    add_library(raytmx INTERFACE)
    target_include_directories(raytmx INTERFACE ${RAYTMX_LOCAL_PATH})
    target_link_libraries(raytmx INTERFACE hoxml)
else()
    # Fallback: fetch from GitHub if submodule missing
    FetchContent_Declare(
      raytmx
      GIT_REPOSITORY https://github.com/shenZybO/raytmx.git
      GIT_TAG main
    )
    FetchContent_GetProperties(raytmx)
    if(NOT raytmx_POPULATED)
      FetchContent_MakeAvailable(raytmx)
    endif()
    add_library(raytmx INTERFACE)
    target_include_directories(raytmx INTERFACE ${raytmx_SOURCE_DIR})
    target_link_libraries(raytmx INTERFACE hoxml)
endif()

# -------------------------
# Build the game
# -------------------------
add_executable(the_game 
  src/main.cpp
  src/Actors/player.cpp
  src/Actions/move.cpp
  src/Logic/gamelogic.cpp
  src/Input/input_manager.cpp
  src/Helpers/animation2d.cpp
  src/Helpers/animation2d_blinker.cpp
  src/Logic/gamelevel.cpp
  src/Actors/Abilities/Movement/movable.cpp
  src/Actors/Abilities/Movement/jumpable.cpp
  src/Actions/jump.cpp
  src/Actors/Abilities/Movement/patrolable.cpp
  src/Actors/enemy.cpp
  src/Helpers/texture_manager.cpp
  src/Logic/collision_system.cpp
)

# Only need to link raylib + raytmx (hoxml comes automatically)
target_link_libraries(the_game PRIVATE raylib raytmx)

# For Windows: include required libraries
if(WIN32)
  target_link_libraries(the_game PRIVATE winmm)
endif()

# Project include directories (allow including headers with e.g. "Actors/player.h")
target_include_directories(the_game PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/src/Actors
  ${CMAKE_SOURCE_DIR}/src/Actions
  ${CMAKE_SOURCE_DIR}/src/Logic
  ${CMAKE_SOURCE_DIR}/src/Input
  ${CMAKE_SOURCE_DIR}/src/Helpers
  ${CMAKE_SOURCE_DIR}/src/Actors/Abilities
  ${CMAKE_SOURCE_DIR}/src/Actors/Abilities/Movement
)

# -------------------------
# clang-format helper target
# -------------------------
find_program(CLANG_FORMAT_EXE NAMES clang-format clang-format-10 clang-format-11 clang-format-12 clang-format-13)

file(GLOB_RECURSE ALL_CXX_FILES
  RELATIVE ${CMAKE_SOURCE_DIR}
  "src/*.cpp" "src/*.h" "include/*.h"
)

if(CLANG_FORMAT_EXE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format on source files"
  )
else()
  add_custom_target(format
    COMMAND ${CMAKE_COMMAND} -E echo "clang-format not found in PATH. Install clang-format to enable the 'format' target."
    COMMENT "clang-format not available"
  )
endif()