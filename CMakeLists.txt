cmake_minimum_required(VERSION 3.16)
project(the_game)

set(CMAKE_CXX_STANDARD 17)

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
  FetchContent_Populate(hoxml)
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
      GIT_REPOSITORY https://github.com/yourname/raytmx.git
      GIT_TAG main
    )
    FetchContent_GetProperties(raytmx)
    if(NOT raytmx_POPULATED)
      FetchContent_Populate(raytmx)
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
  src/player.cpp
  src/move.cpp
  src/gamelogic.cpp
  src/input_manager.cpp
  src/animation2d.cpp
  src/gamelevel.cpp
  src/movable.cpp
)

# Only need to link raylib + raytmx (hoxml comes automatically)
target_link_libraries(the_game PRIVATE raylib raytmx)

# For Windows: include required libraries
if(WIN32)
  target_link_libraries(the_game PRIVATE winmm)
endif()

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