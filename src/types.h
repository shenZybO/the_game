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
    };
}