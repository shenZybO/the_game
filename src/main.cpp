#include "raylib.h"
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"
#include "config.hpp"
#include "player.h"
#include "gamelogic.h"
#include "gamelevel.h"
#include "asset_manager.h"
#include "enemy.h"
#include "texture_manager.h"

/**
 * @brief Program entry: initializes systems, creates a level and runs the main loop.
 */
int main(int argc, char** argv) {
    using namespace std::filesystem;
    // Find executable path
    path exePath = canonical(argv[0]).parent_path();

    // Set assets relative to executable
    AssetManager::SetAssetRoot(exePath / GameConfig::RESOURCES_PATH);

    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "THE GAME");
    SetTargetFPS(Config::TARGET_FPS);

    // create the first level (just demo level) - will add level switching and simple menu later
    GameLevel gameLevel0{GameConfig::LEVELS[0]};

    while (!WindowShouldClose() && !gameLevel0.IsGameOver()) {
        // Poll input and dispatch events
        InputManager::Instance().Update();

        // Update game logic (perform active actions)
        GameLogic::Instance().Update();
        // Update all actors
        gameLevel0.UpdateAll();
        // Render the game level
        gameLevel0.Render();
    }

    // Cleanup and close
    GameLogic::Instance().Cleanup();

    UnloadTMX(gameLevel0.GetMap());
    TextureManager::Instance().UnloadAll();
    CloseWindow();
    return 0;
}
