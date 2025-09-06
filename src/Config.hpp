#pragma once

#include <string_view>

namespace Config {
    inline constexpr int SCREEN_WIDTH  = 1024;
    inline constexpr int SCREEN_HEIGHT = 768;
    inline constexpr int TARGET_FPS = 60;
}

namespace MoveConfig {
    inline constexpr int VERTICAL_SNAP_TOLERANCE = 4;
    inline constexpr float GRAVITY_CONSTANT = 800.0f; // pixels per second squared
}

namespace PlayerConfig {
    inline constexpr float DEFAULT_JUMP_STRENGTH = 400.0f;
    inline constexpr float DEFAULT_MOVE_SPEED = 200.0f;
    inline constexpr std::string_view IDLE_ANIM = "../resources/player_idle.png";
    inline constexpr int IDLE_ANIM_FRAMES = 2;
    inline constexpr float IDLE_ANIM_FRAME_DURATION = 1.0f; // seconds per frame
    inline constexpr std::string_view WALK_ANIM = "../resources/player_walk.png";
    inline constexpr int WALK_ANIM_FRAMES = 2;
    inline constexpr float WALK_ANIM_FRAME_DURATION = 0.2f; // seconds per frame
}