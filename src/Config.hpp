#pragma once

#include "types.h"
#include <string_view>
#include <array>

namespace Config {
    inline constexpr int SCREEN_WIDTH  = 1024;
    inline constexpr int SCREEN_HEIGHT = 768;
    inline constexpr int TARGET_FPS = 60;
}

namespace GameConfig {
    inline constexpr std::array LEVELS {
        std::string_view{"maps/lvl_0.tmx"},
        std::string_view{"maps/lvl_1.tmx"},
    };
}

namespace MoveConfig {
    inline constexpr int VERTICAL_SNAP_TOLERANCE = 4;
    inline constexpr float GRAVITY_CONSTANT = 800.0f; // pixels per second squared
}

namespace PlayerConfig {
    inline constexpr float DEFAULT_JUMP_STRENGTH = 400.0f;
    inline constexpr float DEFAULT_MOVE_SPEED = 200.0f;
    inline constexpr GameTypes::AnimationData IDLE_ANIM { "sprites/player_idle.png", 2, 1.0f }; // path, frames, seconds per frame
    inline constexpr GameTypes::AnimationData WALK_ANIM { "sprites/player_walk.png", 2, 0.2f }; // path, frames, seconds per frame
    inline constexpr GameTypes::AnimationData JUMP_ANIM { "sprites/player_jump.png", 1, 0.1f }; // path, frames, seconds per frame
    inline constexpr GameTypes::AnimationData FALL_ANIM { "sprites/player_fall.png", 1, 0.1f }; // path, frames, seconds per frame
    inline constexpr bool CAN_DOUBLE_JUMP = true;  // If true, grant exactly one extra jump while airborne (total 2 jumps)
}

namespace EnemyConfig {
    inline constexpr float DEFAULT_MOVE_SPEED = 80.0f;
    inline constexpr GameTypes::AnimationData IDLE_ANIM { "sprites/zombie_idle.png", 1, 0.1f }; // path, frames, seconds per frame
    inline constexpr GameTypes::AnimationData WALK_ANIM { "sprites/zombie_walk.png", 2, 0.2f }; // path, frames, seconds per frame
}