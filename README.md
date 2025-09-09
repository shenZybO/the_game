# the_game

A simple 2D game starter project in C++ using raylib.

## Requirements
- CMake 3.5+
- A C++17 compatible compiler (MSVC, MinGW, etc.)
- Git

## Build Instructions (Windows)

1. Open a terminal in the project directory.
2. Run:
   ```
   mkdir build
   # the_game

   A small 2D platformer prototype implemented in modern C++ (C++17) as a learning exercise.

   Status
   -- This project is a work in progress and was developed purely to learn modern C++ and simple game programming patterns. It may never reach a "finished" state.

   Libraries and resources
   - raylib — simple and portable C library for games and multimedia (graphics, input, audio).
   - raytmx — helper for loading/drawing TMX maps (tilemap support).
   - Free 2D assets from Kenney: https://kenney.nl/assets/ (used for tiles, sprites and UI art).

   Requirements
   - CMake 3.16+ (project uses FetchContent)
   - A C++17 compatible compiler (MSVC, MinGW, clang, gcc)
   - Git

   Build (Windows - PowerShell)
   ```powershell
   mkdir build; cd build
   cmake ..
   cmake --build . --config Debug
   ```

   Run
   ```powershell
   # Debug build
   .\build\Debug\the_game.exe
   # or Release build
   .\build\Release\the_game.exe
   ```

   Basic Controls
   - Left/Right arrows — move
   - Up / Space — jump
   - Escape — quit

   TODO
   - Polish collision resolution (multi-tile collisions / slopes)
   - Improve enemy AI and add more enemy types
   - Add level objects and interactive tiles (moving platforms, switches)
   - Integrate a level editor or import object layers from TMX files
   - Add audio (music, SFX) and volume controls
   - Add unit/integration tests for core gameplay code
   - Improve build scripts and CI (cross-platform builds)

   Notes
   - Assets and third-party libraries are included or referenced for convenience. Check their respective licenses before redistribution.
