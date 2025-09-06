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
    Player& p = gameLevel0.addActor<Player>(100.0f /* initial x */, 100.0f /* initial y */, 
                                            PlayerConfig::DEFAULT_JUMP_STRENGTH,
                                            PlayerConfig::DEFAULT_MOVE_SPEED,
                                            PlayerConfig::IDLE_ANIM.data(),
                                            PlayerConfig::IDLE_ANIM_FRAMES,
                                            PlayerConfig::IDLE_ANIM_FRAME_DURATION,
                                            PlayerConfig::WALK_ANIM.data(),
                                            PlayerConfig::WALK_ANIM_FRAMES,
                                            PlayerConfig::WALK_ANIM_FRAME_DURATION);


    // Player& p = gameLevel0.addActor<Player>(100.0f /* initial x */, 100.0f /* initial y */, 
    //                                         PlayerConfig::DEFAULT_JUMP_STRENGTH,
    //                                         PlayerConfig::IDLE_ANIM.data(),
    //                                         PlayerConfig::IDLE_ANIM_FRAMES,
    //                                         PlayerConfig::IDLE_ANIM_FRAME_DURATION);

    // dynamic_cast<Movable&>(p).SetMovingAnimation(std::make_unique<Animation2D>(
    //     PlayerConfig::WALK_ANIM.data(), PlayerConfig::WALK_ANIM_FRAMES, PlayerConfig::WALK_ANIM_FRAME_DURATION));

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
