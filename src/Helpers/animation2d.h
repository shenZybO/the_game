#pragma once

#include <string_view>
#include "raylib.h"
#include "types.h"
#include "ianimation2d.h"

/**
 * @brief Simple 2D animation helper managing frames and drawing.
 *
 * Loads a texture and provides frame timing, update and draw helpers. The
 * class may own the underlying `Texture2D` (and will unload it on destruction)
 * depending on the constructor used.
 */
class Animation2D : public IAnimation2D {
public:
    /**
     * @brief Construct an Animation2D and load a texture from file.
     *
     * The Animation2D instance takes ownership of the loaded texture.
     */
    Animation2D(GameTypes::AnimationData animData);

    /**
     * @brief Construct using an existing Texture2D.
     *
     * When ownsTexture is true the destructor will unload the texture.
     */
    Animation2D(Texture2D& texture, int frameCount = 1, float frameDuration = 0.1f);

    /**
     * @brief Destroy the Animation2D and free owned resources.
     */
    ~Animation2D();

    /**
     * @brief Advance the animation by delta seconds.
     *
     * @param delta Elapsed time in seconds since last update.
     */
    void Update(float delta) override;

    /**
     * @brief Draw the current animation frame at the given position.
     *
     * @param position World position to draw the sprite.
     * @param flipped If true the sprite will be flipped horizontally.
     * @param tint Color tint to apply to the sprite.
     * @param scale Uniform scale factor for drawing.
     */
    void Draw(Vector2 position, bool flipped, Color tint = WHITE, float scale = 1.0f) const override;

    /**
     * @brief Get the total number of frames.
     *
     * @return int Frame count.
     */
    int GetFrameCount() const { return frameCount; }

    /**
     * @brief Get the configured frame duration (seconds).
     */
    float GetFrameDuration() const { return frameDuration; }

    /**
     * @brief Width of a single animation frame in pixels.
     */
    float GetFrameWidth() const override {
        if (!texture) return 0.0f;
        return (frameCount > 0) ? ((float)texture->width / (float)frameCount) : (float)texture->width;
    }

    /**
     * @brief Height of a single animation frame in pixels.
     */
    float GetFrameHeight() const override {
        if (!texture) return 0.0f;
        return (float)texture->height;
    }

    /**
     * @brief Set per-animation draw offset in pixels.
     *
     * Offset shifts where the sprite is drawn relative to the actor's origin.
     */
    void SetDrawOffset(Vector2 offset) noexcept override { drawOffset = offset; }

    /**
     * @brief Get the current draw offset in pixels.
     */
    Vector2 GetDrawOffset() const noexcept override { return drawOffset; }

private:
    Texture2D* texture;  /**< Non-owning pointer to underlying texture. */
    int frameCount;      /**< Number of frames in the texture horizontally. */
    float frameDuration; /**< Duration in seconds per frame. */
    int currentFrame;    /**< Index of the current frame. */
    float timer;         /**< Accumulated time used to advance frames. */
    // no ownership; textures are managed by TextureManager
    Vector2 drawOffset{0.0f, 0.0f}; /**< Visual offset applied when drawing. */
};