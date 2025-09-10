#pragma once

#include <cstdint>
#include <string_view>

/**
 * @brief Common game-related types and small PODs used across the project.
 */
namespace GameTypes {
    enum class Direction : std::uint8_t { Left = 0, Right = 1 };

    struct AnimationData {
        std::string_view texturePath;
        int frameCount;
        float frameDuration;
        float offsetX = 0.0f; /**< Optional screen-space draw offset X (pixels); +X moves right. */
        float offsetY = 0.0f; /**< Optional screen-space draw offset Y (pixels); +Y moves down. */    
    };
}