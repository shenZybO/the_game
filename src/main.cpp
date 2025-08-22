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

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Poll input and dispatch events
        InputManager::Instance().Update();

        // Update game logic (perform active actions)
        GameLogic::Instance().Update(delta);
        // Update all actors
        gameLevel0.UpdateAll(delta);
        // Render the game level
        gameLevel0.Render();
    }

    UnloadTMX(gameLevel0.GetMap());
    CloseWindow();
    return 0;
}
