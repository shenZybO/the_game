#include "animation2d.h"

Animation2D::Animation2D(const char* imagePath, int frameCount, float frameDuration)
    : texture(LoadTexture(imagePath)),
      frameCount(frameCount > 0 ? frameCount : 1),
      frameDuration(frameDuration > 0.0f ? frameDuration : 0.1f),
      currentFrame(0),
      timer(0.0f),
      ownsTexture(true) {}

Animation2D::Animation2D(Texture2D texture, int frameCount, float frameDuration, bool ownsTexture)
    : texture(texture),
      frameCount(frameCount > 0 ? frameCount : 1),
      frameDuration(frameDuration > 0.0f ? frameDuration : 0.1f),
      currentFrame(0),
      timer(0.0f),
      ownsTexture(ownsTexture) {}

Animation2D::~Animation2D() {
    if (ownsTexture) {
        // debug output
        TraceLog(LOG_INFO, "Animation2D destructor: Unloading texture with ID %u", texture.id);
        UnloadTexture(texture);
    }
}

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

void Animation2D::Draw(Vector2 position, Color tint, float scale) const {
    Rectangle srcRec{};
    Rectangle dstRec{};

    if (frameCount <= 1) {
        // static texture; use DrawTexturePro to support flipping via negative source width
        srcRec = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        if (flipped) {
            srcRec.x = (float)texture.width;  // start at right edge
            srcRec.width = -srcRec.width;     // negative width flips horizontally
        }

        dstRec = {position.x, position.y, (float)texture.width * scale,
                  (float)texture.height * scale};
    } else {
        // animated texture; calculate source rectangle based on current frame
        float frameWidth = (float)texture.width / (float)frameCount;
        float frameHeight = (float)texture.height;

        if (!flipped) {
            srcRec = {frameWidth * currentFrame, 0.0f, frameWidth, frameHeight};
        } else {
            // start from right edge of the current frame and use negative width to flip
            srcRec = {frameWidth * (currentFrame + 1), 0.0f, -frameWidth, frameHeight};
        }

        dstRec = {position.x, position.y, frameWidth * scale, frameHeight * scale};
    }

    Vector2 origin = {0.0f, 0.0f};
    DrawTexturePro(texture, srcRec, dstRec, origin, 0.0f, tint);
    return;
}