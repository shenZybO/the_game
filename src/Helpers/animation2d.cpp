#include "types.h"
#include "animation2d.h"
#include "asset_manager.h"
#include "texture_manager.h"

/**
 * @brief Construct Animation2D by loading texture from asset path.
 *
 * The constructor resolves the texture path via AssetManager and loads it.
 */
Animation2D::Animation2D(GameTypes::AnimationData animData)
        : texture(&TextureManager::Instance().GetTexture(animData.texturePath)),
            frameCount(animData.frameCount > 0 ? animData.frameCount : 1),
            frameDuration(animData.frameDuration > 0.0f ? animData.frameDuration : 0.1f),
            currentFrame(0),
            timer(0.0f), 
            drawOffset(animData.offsetX, animData.offsetY) {}

/**
 * @brief Construct Animation2D from an existing Texture2D.
 *
 * When ownsTexture is true the destructor will unload the texture.
 */
Animation2D::Animation2D(Texture2D& texture, int frameCount, float frameDuration)
        : texture(&texture),
            frameCount(frameCount > 0 ? frameCount : 1),
            frameDuration(frameDuration > 0.0f ? frameDuration : 0.1f),
            currentFrame(0),
            timer(0.0f), 
            drawOffset(0.0f, 0.0f) {}

/**
 * @brief Destroy the Animation2D and free owned resources.
 */
Animation2D::~Animation2D() { }

/**
 * @brief Advance the animation timer and choose the current frame.
 *
 * Handles large deltas by advancing multiple frames when necessary.
 */
void Animation2D::Update(float delta) {
    if (frameCount > 1)  // only update if there are multiple frames
    {
        timer += delta;
        // advance frames while timer exceeds frameDuration (handles large delta)
        while (timer >= frameDuration) {
            timer -= frameDuration;
            currentFrame = (currentFrame + 1) % frameCount;
        }
    }
    return;
}

/**
 * @brief Draw the current frame using raylib's DrawTexturePro.
 */
void Animation2D::Draw(Vector2 position, bool flipped, Color tint, float scale) const {
    Rectangle srcRec{};
    Rectangle dstRec{};

    /* Apply visual draw offset before computing destination rectangle */
    Vector2 adjusted {
        position.x + (flipped ? -drawOffset.x : drawOffset.x) * scale,
        position.y + drawOffset.y * scale
    };

    if (frameCount <= 1) {
        // static texture; use DrawTexturePro to support flipping via negative source width
        srcRec = {0.0f, 0.0f, (float)texture->width, (float)texture->height};
        if (flipped) {
            srcRec.x = (float)texture->width;  // start at right edge
            srcRec.width = -srcRec.width;     // negative width flips horizontally
        }

        dstRec = {adjusted.x, adjusted.y, (float)texture->width * scale,
                  (float)texture->height * scale};
    } else {
        // animated texture; calculate source rectangle based on current frame
        float frameWidth = (float)texture->width / (float)frameCount;
        float frameHeight = (float)texture->height;

        if (!flipped) {
            srcRec = {frameWidth * currentFrame, 0.0f, frameWidth, frameHeight};
        } else {
            // start from right edge of the current frame and use negative width to flip
            srcRec = {frameWidth * (currentFrame + 1), 0.0f, -frameWidth, frameHeight};
        }

        dstRec = {adjusted.x, adjusted.y, frameWidth * scale, frameHeight * scale};
    }

    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(*texture, srcRec, dstRec, origin, 0.0f, tint);
}