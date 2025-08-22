#pragma once

#include "raylib.h"

class Animation2D {
   public:
    // Load from file (Animation2D takes ownership of the loaded texture)
    Animation2D(const char* imagePath, int frameCount = 1, float frameDuration = 0.1f);

    // Use an existing texture. If ownsTexture is true, the destructor will unload the texture.
    Animation2D(Texture2D texture, int frameCount = 1, float frameDuration = 0.1f,
                bool ownsTexture = false);

    ~Animation2D();

    // Advance animation based on elapsed time (seconds)
    void Update(float delta);

    // Draw current frame at position. Uses scale to resize sprite.
    // If 'flipped' is true the sprite is drawn mirrored horizontally (left-facing).
    void Draw(Vector2 position, Color tint = WHITE, float scale = 1.0f, bool flipped = false) const;

    // Accessors
    int GetFrameCount() const { return frameCount; }
    float GetFrameDuration() const { return frameDuration; }

    // Get current frame dimensions (in pixels)
    float GetFrameWidth() const {
        return (frameCount > 0) ? ((float)texture.width / (float)frameCount) : (float)texture.width;
    }
    float GetFrameHeight() const { return (float)texture.height; }

   private:
    Texture2D texture;
    int frameCount;
    float frameDuration;

    int currentFrame;
    float timer;

    bool ownsTexture;
};