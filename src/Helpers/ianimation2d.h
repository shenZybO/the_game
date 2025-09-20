#pragma once

#include "raylib.h"

/**
 * @brief Common interface for 2D animations used by actors.
 *
 * Implemented by concrete Animation2D and decorator(s) like BlinkingAnimation2D.
 */
class IAnimation2D {
public:
    virtual ~IAnimation2D() = default;

    /** Advance internal state by delta seconds. */
    virtual void Update(float delta) = 0;

    /** Draw at position with optional flip, tint and scale. */
    virtual void Draw(Vector2 position, bool flipped, Color tint = WHITE, float scale = 1.0f) const = 0;

    /** Frame size of the current animation frame (in pixels before scale). */
    virtual float GetFrameWidth() const = 0;
    virtual float GetFrameHeight() const = 0;

    /** Optional drawing offset relative to position. */
    virtual void SetDrawOffset(Vector2 offset) = 0;
    virtual Vector2 GetDrawOffset() const = 0;
};
