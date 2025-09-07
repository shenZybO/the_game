#pragma once

#include <cstdint>
#include <string_view>

namespace GameTypes {
    enum class Direction : std::uint8_t { Left = 0, Right = 1 };

    struct AnimationData {
        std::string_view texturePath;
        int frameCount;
        float frameDuration;
    };
}