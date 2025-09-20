#include "animation2d_blinker.h"
#include "animation2d.h"
#include "ianimation2d.h"

#include <algorithm>

namespace {
/* Clamp a float into [lo, hi] - float specific clamp */
constexpr float clampf(float value, float lo, float hi) noexcept {
    return (value < lo) ? lo : (value > hi) ? hi : value;
}

/* Multiply alpha value [0..255] by a factor [0..1], clamp result to [0..255] */
unsigned char multiplyAlpha(unsigned char baseAlpha, float factor) noexcept {
    float result = static_cast<float>(baseAlpha) * clampf(factor, 0.0f, 1.0f);
    result = clampf(result, 0.0f, 255.0f);
    return static_cast<unsigned char>(result + 0.5f);
}
}  // namespace

BlinkingAnimation2D::BlinkingAnimation2D(std::shared_ptr<IAnimation2D> wrapped, float visibleDurationSeconds,
                                         float fadedDurationSeconds, float minAlpha) noexcept
    : anim(std::move(wrapped)),
      visibleDuration(visibleDurationSeconds > 0.01f ? visibleDurationSeconds : 0.01f),
      fadedDuration(fadedDurationSeconds > 0.01f ? fadedDurationSeconds : 0.01f),
      minBlinkAlpha(clampf(minAlpha, 0.0f, 1.0f)),
      timer(0.0f),
      visiblePhase(true) {}

BlinkingAnimation2D::BlinkingAnimation2D(std::shared_ptr<IAnimation2D> wrapped, float blinkDurationSeconds,
                                         float minAlpha) noexcept
    : BlinkingAnimation2D(std::move(wrapped), blinkDurationSeconds, blinkDurationSeconds, minAlpha) {}

void BlinkingAnimation2D::Update(float delta) {
    if (!anim) return;
    anim->Update(delta);

    timer += delta;
    const float phaseDuration = visiblePhase ? visibleDuration : fadedDuration;
    /* Flip phase when the current duration elapses; carry over extra time to avoid drift */
    if (timer >= phaseDuration) {
        timer -= phaseDuration;
        visiblePhase = !visiblePhase;
    }
}

void BlinkingAnimation2D::Draw(Vector2 position, bool flipped, Color tint, float scale) const {
    if (!anim) return;

    const float alphaMul = visiblePhase ? 1.0f : minBlinkAlpha;
    Color modTint = tint;
    modTint.a = multiplyAlpha(tint.a, alphaMul);

    anim->Draw(position, flipped, modTint, scale);
}

void BlinkingAnimation2D::Configure(float visibleDurationSeconds, float fadedDurationSeconds, float minAlpha) {
    visibleDuration = (visibleDurationSeconds > 0.01f) ? visibleDurationSeconds : 0.01f;
    fadedDuration = (fadedDurationSeconds > 0.01f) ? fadedDurationSeconds : 0.01f;
    minBlinkAlpha = clampf(minAlpha, 0.0f, 1.0f);
}

float BlinkingAnimation2D::GetFrameWidth() const {
    return anim ? anim->GetFrameWidth() : 0.0f;
}

float BlinkingAnimation2D::GetFrameHeight() const {
    return anim ? anim->GetFrameHeight() : 0.0f;
}

void BlinkingAnimation2D::SetDrawOffset(Vector2 offset) {
    if (anim) anim->SetDrawOffset(offset);
}

Vector2 BlinkingAnimation2D::GetDrawOffset() const {
    return anim ? anim->GetDrawOffset() : Vector2{0.0f, 0.0f};
}
