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
    inline constexpr std::string_view RESOURCES_PATH = "../../resources";
}

namespace MoveConfig {
    inline constexpr int VERTICAL_SNAP_TOLERANCE = 4;
    inline constexpr float GRAVITY_CONSTANT = 800.0f; // pixels per second squared
    // Foot sensor shape and grounding hysteresis
    inline constexpr float FOOT_SENSOR_WIDTH_RATIO = 0.6f;   // 0.2..1.0; narrower avoids edge flicker
    inline constexpr float FOOT_SENSOR_HEIGHT = 2.0f;        // thin strip below feet
    inline constexpr float FOOT_SENSOR_GAP = 1.0f;           // small gap below collider
    inline constexpr float GROUND_GRACE_TIME = 0.08f;        // seconds; prevents rapid ground toggle
    inline constexpr float FOOT_SENSOR_MIN_WIDTH_RATIO = 0.2f; // minimum width ratio to avoid too narrow sensor
}

namespace PlayerConfig {
    inline constexpr float DEFAULT_JUMP_STRENGTH = 400.0f;
    inline constexpr float DEFAULT_MOVE_SPEED = 200.0f;
    // AnimationData: path, frames, seconds per frame, offsetX, offsetY
    inline constexpr GameTypes::AnimationData IDLE_ANIM { "sprites/player_idle.png", 2, 1.0f, 0.0f, 0.0f };
    inline constexpr GameTypes::AnimationData WALK_ANIM { "sprites/player_walk.png", 2, 0.2f, 2.0f, 0.0f };
    inline constexpr GameTypes::AnimationData JUMP_ANIM { "sprites/player_jump.png", 1, 0.1f, 3.0f, 0.0f };
    inline constexpr GameTypes::AnimationData FALL_ANIM { "sprites/player_fall.png", 1, 0.1f, 5.0f, 0.0f };
    inline constexpr bool CAN_DOUBLE_JUMP = true;  // If true, grant exactly one extra jump while airborne (total 2 jumps)

    // Default physics collider (tune as needed to fit art); values in pixels
    inline constexpr float COLLIDER_WIDTH = 48.0f;
    inline constexpr float COLLIDER_HEIGHT = 72.0f;
    inline constexpr float COLLIDER_OFFSET_X = 2.0f;  // from sprite top-left to collider top-left
    inline constexpr float COLLIDER_OFFSET_Y = 0.0f;
}

namespace EnemyConfig {
    inline constexpr float DEFAULT_MOVE_SPEED = 80.0f;
    inline constexpr GameTypes::AnimationData IDLE_ANIM { "sprites/zombie_idle.png", 1, 0.1f, 0.0f, 0.0f };
    inline constexpr GameTypes::AnimationData WALK_ANIM { "sprites/zombie_walk.png", 2, 0.2f, 3.0f, 0.0f };

    // Default collider for enemy; tune to sprite
    inline constexpr float COLLIDER_WIDTH = 48.0f;
    inline constexpr float COLLIDER_HEIGHT = 72.0f;
    inline constexpr float COLLIDER_OFFSET_X = 2.0f;
    inline constexpr float COLLIDER_OFFSET_Y = 0.0f;
}