cmake_minimum_required(VERSION 4.0)
project(the_game)

set(CMAKE_CXX_STANDARD 17)

# Download and add raylib
include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Fetch raytmx.h from GitHub
FetchContent_Declare(
  raytmx
  GIT_REPOSITORY https://github.com/luphi/raytmx.git
  GIT_TAG main
)
FetchContent_MakeAvailable(raytmx)

# Fetch hoxml (raytmx dependency)
FetchContent_Declare(
  hoxml
  GIT_REPOSITORY https://github.com/luphi/hoxml.git
  GIT_TAG main
)
FetchContent_MakeAvailable(hoxml)

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
target_link_libraries(the_game PRIVATE raylib)

# For Windows: include required libraries
if (WIN32)
    target_link_libraries(the_game PRIVATE winmm)
endif()

# Add raytmx and hoxml include directories (header-only)
target_include_directories(the_game PRIVATE ${raytmx_SOURCE_DIR} ${hoxml_SOURCE_DIR})

# Optional: add a 'format' target that runs clang-format on C/C++ sources
find_program(CLANG_FORMAT_EXE NAMES clang-format clang-format-10 clang-format-11 clang-format-12 clang-format-13)

# Collect source files to format at configure time
file(GLOB_RECURSE ALL_CXX_FILES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "src/*.h" "include/*.h")

# Always create a 'format' target so build systems (ninja/msbuild) expose it.
if(CLANG_FORMAT_EXE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${ALL_CXX_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format on source files"
  )
else()
  # clang-format not found: create a no-op target that prints guidance instead of being missing
  add_custom_target(format
    COMMAND ${CMAKE_COMMAND} -E echo "clang-format not found in PATH. Install clang-format to enable the 'format' target."
    COMMENT "clang-format not available"
  )
endif()
