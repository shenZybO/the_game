# the_game

A small 2D platformer prototype implemented in modern C++ (C++20) as a learning exercise.

## Status
   -- This project is a work in progress and was developed purely to learn modern C++ and simple game programming patterns. It may never reach a "finished" state.

## Libraries and resources
   - raylib — simple and portable C library for games and multimedia (graphics, input, audio).
   - raytmx — helper for loading/drawing TMX maps (tilemap support).
   - Free 2D assets from Kenney: https://kenney.nl/assets/ (used for tiles, sprites and UI art).

## Requirements
   - CMake 3.16+ (project uses FetchContent)
   - A C++20 compatible compiler (MSVC, MinGW, clang, gcc)
   - Git

## TODOs
   - Health system for player with collecting health (including graphical representation)
   - Implement dying of actors - level restart / game over [IN PROGRESS]
   - Add way to complete a level by reaching an exit point
   - Resolve collision between player and enemies
   - Fill levels with actors based on object layers in TMX maps [DONE]
   - Implement more abilities for actors: climbing ladders, throwing rocks, shooting projectiles
   - Add simple game menu
   - Add collectible objects and game score
   - More complex interaction - e.g. switches start actions like moving a platform

## Notes
   - Assets and third-party libraries are included or referenced for convenience. Check their respective licenses before redistribution.
