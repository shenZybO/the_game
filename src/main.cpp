#include "raylib.h"
#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"
#include "Config.hpp"
#include "player.h"
#include "gamelogic.h"
#include "gamelevel.h"

int main() {
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "THE GAME");
    SetTargetFPS(Config::TARGET_FPS);

    GameLevel gameLevel0("../resources/maps/lvl_0.tmx");
    // Add a player actor to the game level
    Player& p = gameLevel0.addActor<Player>(100.0f, 100.0f, 200.0f, 400.0f,
                                            "../resources/player_idle.png", 2, 1.0f);

    p.SetMovingAnimation(std::make_unique<Animation2D>(
        "../resources/player_walk.png", 2, 0.2f));  // 4 frames, 0.1s per frame

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
