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

    // TODO: level construction (load map, add actors, etc) will be handled 
    // based on object layers data in the TMX map later

    GameLevel gameLevel0(GameConfig::LEVELS[0]);
    // Add a player actor to the game level
    Player& p = gameLevel0.addActor<Player>(100.0f /* initial x */, 100.0f /* initial y */, 
                                            PlayerConfig::DEFAULT_JUMP_STRENGTH,
                                            PlayerConfig::DEFAULT_MOVE_SPEED,
                                            PlayerConfig::IDLE_ANIM,
                                            PlayerConfig::WALK_ANIM,
                                            PlayerConfig::JUMP_ANIM,
                                            PlayerConfig::FALL_ANIM);

    // Add a simple Enemy to the level (spawn above ground)
    // Enemy will start falling and then patrol left/right when grounded.
    gameLevel0.addActor<Enemy>(450.0f /* x */, 250.0f /* y */, EnemyConfig::DEFAULT_MOVE_SPEED,
                                         EnemyConfig::IDLE_ANIM, EnemyConfig::WALK_ANIM);

    // add another enemy to test shared texture via TextureManager
    gameLevel0.addActor<Enemy>(750.0f /* x */, 250.0f /* y */, EnemyConfig::DEFAULT_MOVE_SPEED,
                                          EnemyConfig::IDLE_ANIM, EnemyConfig::WALK_ANIM);

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

    // Cleanup and close
    GameLogic::Instance().Cleanup();

    UnloadTMX(gameLevel0.GetMap());
    TextureManager::Instance().UnloadAll();
    CloseWindow();
    return 0;
}
