#pragma once

#include "raytmx.h"
#include "animation2d.h"
#include <memory>

// Forward declare GameLevel (reference only needs this)
class GameLevel;

/* Abstract base class for all game actors - game objects which can perform actions (including game
 * characters and items) Actor needs a posiotion, size and grapical representation.
 */
class Actor {
   public:
    // Default ctor - keeps compatibility with existing code that constructs Actor without params
    Actor() = default;

    // Construct with an owned Animation2D (takes ownership)
    Actor(GameLevel& level, std::unique_ptr<Animation2D> anim, float x = 0.0f, float y = 0.0f)
        : position{x, y}, defaultAnimation(std::move(anim)), gameLevel(level) {}

    // Construct by providing animation parameters; Actor will create its own Animation2D
    Actor(GameLevel& level, const char* imagePath, int frameCount = 1, float frameDuration = 0.1f,
          float x = 0.0f, float y = 0.0f)
        : position{x, y},
          defaultAnimation(std::make_unique<Animation2D>(imagePath, frameCount, frameDuration)),
          gameLevel(level) {}

    ~Actor() {
        defaultAnimation.reset();  // Ensure proper cleanup of the unique_ptr
    }

    // Accessor for alive state
    bool IsAlive() const { return alive; }

    // Method to destroy the actor
    void Destroy() {
        alive = false;  // Mark the actor as not alive
    }

    // Accessor for position
    Vector2 GetPosition() const { return position; }

    // Accessor for the actor's rectangle
    Rectangle GetRect() const {
        if (defaultAnimation) {
            float frameWidth = defaultAnimation->GetFrameWidth();
            float frameHeight = defaultAnimation->GetFrameHeight();
            return {position.x, position.y, frameWidth, frameHeight};
        }
        return {position.x, position.y, 10.0f, 10.0f};  // Default rectangle if no animation
    }

    // Accessor for the current animation
    const Animation2D* GetAnimation() const { return defaultAnimation.get(); }

    /** Base implementation of Update method
     * Derived classes can override this to implement specific behavior
     */
    virtual void Update(float delta) {
        if (defaultAnimation) {
            defaultAnimation->Update(delta);
        }
    };

    // Draw the actor
    virtual void Draw() {
        if (defaultAnimation) {
            defaultAnimation->Draw(position);
        }
    }

   protected:
    Vector2 position;
    std::unique_ptr<Animation2D> defaultAnimation;  // optional owned animation
    GameLevel& gameLevel;  // non-owning reference to the current game level
    bool alive = true;     // flag to indicate if the actor is still alive (meaning not destroyed)
};