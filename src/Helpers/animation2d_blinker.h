#pragma once

#include <memory>
#include "raylib.h"
#include "ianimation2d.h"

class Animation2D;  // fwd decl

/**
 * @brief Decorator that adds a blinking (hard toggle) alpha effect to an Animation2D.
 *
 * The decorator wraps an existing Animation2D and toggles its opacity between
 * fully visible (alpha = 1.0) and a configured faded opacity (alpha = minAlpha),
 * with independently configurable durations for each phase.
 */
class BlinkingAnimation2D : public IAnimation2D {
public:
    /**
     * @brief Construct a blinking decorator with independent phase durations.
     *
     * @param wrapped The animation instance to decorate. Must be non-null.
     * @param visibleDurationSeconds Duration in seconds for the fully visible phase (> 0).
     * @param fadedDurationSeconds Duration in seconds for the faded phase (> 0).
     * @param minAlpha Minimum opacity in range [0.0, 1.0] used during faded phase.
     */
    BlinkingAnimation2D(std::shared_ptr<IAnimation2D> wrapped, float visibleDurationSeconds, float fadedDurationSeconds,
                        float minAlpha) noexcept;

    /**
     * @brief Construct with symmetric on/off durations.
     * @param wrapped Animation to decorate.
     * @param blinkDurationSeconds Duration for each phase (visible and faded).
     * @param minAlpha Minimum opacity during faded phase.
     */
    BlinkingAnimation2D(std::shared_ptr<IAnimation2D> wrapped, float blinkDurationSeconds = 0.3f,
                        float minAlpha = 0.2f) noexcept;

    /**
     * @brief Advance internal timer and the wrapped animation.
     * @param delta Seconds since last update.
     */
    void Update(float delta) override;

    /**
     * @brief Draw the wrapped animation with a toggled alpha applied.
     *
     * @param position Target world position.
     * @param flipped Horizontal flip.
     * @param tint Base tint; its alpha will be multiplied by the phase alpha.
     * @param scale Uniform scale factor.
     */
    void Draw(Vector2 position, bool flipped, Color tint = WHITE, float scale = 1.0f) const override;

    /**
     * @brief Change blink parameters at runtime.
     * @param visibleDurationSeconds Visible phase duration (seconds), clamped to epsilon minimum.
     * @param fadedDurationSeconds Faded phase duration (seconds), clamped to epsilon minimum.
     * @param minAlpha Minimum alpha [0..1]. Values are clamped.
     */
    void Configure(float visibleDurationSeconds, float fadedDurationSeconds, float minAlpha);

    /** Getters for current configuration. */
    float GetVisibleDuration() const noexcept { return visibleDuration; }
    float GetFadedDuration() const noexcept { return fadedDuration; }
    float GetMinAlpha() const noexcept { return minBlinkAlpha; }

    /** Get the wrapped animation (for unwrapping). */
    std::shared_ptr<IAnimation2D> GetWrappedAnimation() const noexcept { return anim; }

    // IAnimation2D frame/offset passthroughs
    float GetFrameWidth() const override;
    float GetFrameHeight() const override;
    void SetDrawOffset(Vector2 offset) override;
    Vector2 GetDrawOffset() const override;

private:
    std::shared_ptr<IAnimation2D> anim; /**< Wrapped animation. */
    float visibleDuration;              /**< Seconds for visible phase. */
    float fadedDuration;                /**< Seconds for faded phase. */
    float minBlinkAlpha;                /**< Minimum alpha [0..1] when faded. */
    float timer;                        /**< Accumulated time in seconds for current phase. */
    bool visiblePhase;                  /**< Current phase: true = visible, false = faded. */
};
