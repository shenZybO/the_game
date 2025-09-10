#pragma once

#include <string_view>
#include "raylib.h"

/**
 * @brief Simple 2D animation helper managing frames and drawing.
 *
 * Loads a texture and provides frame timing, update and draw helpers. The
 * class may own the underlying `Texture2D` (and will unload it on destruction)
 * depending on the constructor used.
 */
class Animation2D {
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
    Animation2D(Texture2D texture, int frameCount = 1, float frameDuration = 0.1f,
                bool ownsTexture = false);

    /**
     * @brief Destroy the Animation2D and free owned resources.
     */
    ~Animation2D();

    /**
     * @brief Advance the animation by delta seconds.
     *
     * @param delta Elapsed time in seconds since last update.
     */
    void Update(float delta);

    /**
     * @brief Draw the current animation frame at the given position.
     *
     * @param position World position to draw the sprite.
     * @param flipped If true the sprite will be flipped horizontally.
     * @param tint Color tint to apply to the sprite.
     * @param scale Uniform scale factor for drawing.
     */
    void Draw(Vector2 position, bool flipped, Color tint = WHITE, float scale = 1.0f) const;

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
    float GetFrameWidth() const {
        return (frameCount > 0) ? ((float)texture.width / (float)frameCount) : (float)texture.width;
    }

    /**
     * @brief Height of a single animation frame in pixels.
     */
    float GetFrameHeight() const { return (float)texture.height; }

   private:
    Texture2D texture; /**< Underlying texture used for animation frames. */
    int frameCount;    /**< Number of frames in the texture horizontally. */
    float frameDuration; /**< Duration in seconds per frame. */
    int currentFrame; /**< Index of the current frame. */
    float timer;      /**< Accumulated time used to advance frames. */
    bool ownsTexture; /**< True when this instance owns the texture and should unload it. */
};