#include "raylib.h"
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"
#include "Config.hpp"
#include "player.h"
#include "gamelogic.h"
#include "gamelevel.h"
#include "asset_manager.h"

int main(int argc, char** argv) {
    using namespace std::filesystem;
    // Find executable path
    path exePath = canonical(argv[0]).parent_path();

    // Set assets relative to executable
    AssetManager::SetAssetRoot(exePath / "../resources");

    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "THE GAME");
    SetTargetFPS(Config::TARGET_FPS);

    GameLevel gameLevel0(GameConfig::LEVELS[0]);
    // Add a player actor to the game level
    Player& p = gameLevel0.addActor<Player>(100.0f /* initial x */, 100.0f /* initial y */, 
                                            PlayerConfig::DEFAULT_JUMP_STRENGTH,
                                            PlayerConfig::DEFAULT_MOVE_SPEED,
                                            PlayerConfig::IDLE_ANIM,
                                            PlayerConfig::WALK_ANIM,
                                            PlayerConfig::JUMP_ANIM,
                                            PlayerConfig::FALL_ANIM);

    while (!WindowShouldClose()) {
        // Poll input and dispatch events
        InputManager::Instance().Update();

        // Update game logic (perform active actions)
        GameLogic::Instance().Update();
        // Update all actors
        gameLevel0.UpdateAll();
        // Render the game level
        gameLevel0.Render();
    }

    UnloadTMX(gameLevel0.GetMap());
    CloseWindow();
    return 0;
}
