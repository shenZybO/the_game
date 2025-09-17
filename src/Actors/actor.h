#pragma once

#include "types.h"
#include "raytmx.h"
#include "animation2d.h"
#include <memory>

// Forward declare GameLevel (reference only needs this)
class GameLevel;

/**
 * @brief Abstract base class for all game actors.
 *
 * Actors are game objects that have a position, an optional graphical representation
 * (animations) and can perform actions. This class provides basic state, position
 * and animation management used by concrete actors (players, enemies, items).
 */
class Actor {
public:
    /**
     * @brief Possible runtime states for an actor.
     *
     * Can be extended (jumping, falling, dying, etc.).
     */
    enum ActorState {
        STATE_IDLE = 0,
        STATE_MOVING_LEFT,
        STATE_MOVING_RIGHT,
        STATE_JUMPING,
        STATE_FALLING,
        STATE_DYING
    };

    // Default ctor - keeps compatibility with existing code that constructs Actor without params
    Actor() = default;

    // Construct with an owned Animation2D (takes ownership)
    Actor(GameLevel& level, std::unique_ptr<Animation2D> anim, float x = 0.0f, float y = 0.0f)
        : position{x, y}, defaultAnimation(std::move(anim)), gameLevel(level) {
        currentAnimation = defaultAnimation;
    }

    // Construct by providing animation parameters; Actor will create its own Animation2D
    Actor(GameLevel& level, GameTypes::AnimationData idleAnim, float x = 0.0f, float y = 0.0f)
        : position{x, y}, defaultAnimation(std::make_shared<Animation2D>(idleAnim)), gameLevel(level) {
        currentAnimation = defaultAnimation;
    }

    virtual ~Actor() {
        defaultAnimation.reset();  // Ensure proper cleanup of animation
        currentAnimation.reset();
    }

    /**
     * @brief Set the current runtime state for the actor.
     *
     * @param s New actor state.
     */
    void SetState(ActorState s) { actorState = s; }

    /**
     * @brief Get the current runtime state for the actor.
     *
     * @return ActorState Current state value.
     */
    ActorState GetState() const { return actorState; }

    /**
     * @brief Query whether the actor is alive (not destroyed).
     *
     * @return true when actor is alive.
     */
    bool IsAlive() const { return alive; }

    /**
     * @brief Mark the actor as destroyed.
     *
     * Sets the internal alive flag to false.
     */
    virtual void Destroy() {
        alive = false;  // Mark the actor as not alive
    }

    /**
     * @brief Get current world position of the actor.
     */
    const Vector2& GetPosition() const noexcept { return position; }

    /**
     * @brief Set current world position of the actor.
     */
    void SetPosition(float x, float y) noexcept { position = {x, y}; }

    /**
     * @brief Set current world position of the actor.
     */
    void SetPosition(const Vector2& p) noexcept { position = p; }

    /**
     * @brief Get the actor's bounding rectangle based on its animation frame size.
     *
     * Returns a default small rectangle when no animation is available.
     */
    Rectangle GetRect() const {
        /*
         * Prefer a fixed physics collider when configured; this decouples physics
         * from per-frame sprite sizes and prevents flicker when animations differ
         * in width/height.
         */
        if (colliderSize.x > 0.0f && colliderSize.y > 0.0f) {
            return {position.x + colliderOffset.x, position.y + colliderOffset.y, colliderSize.x, colliderSize.y};
        }

        if (currentAnimation) {
            float frameWidth = currentAnimation->GetFrameWidth();
            float frameHeight = currentAnimation->GetFrameHeight();
            return {position.x, position.y, frameWidth, frameHeight};
        }
        return {position.x, position.y, 10.0f, 10.0f};  // Default rectangle if no animation
    }

    /**
     * @brief Configure a fixed collider box relative to the actor origin.
     */
    void SetCollider(float offsetX, float offsetY, float width, float height) {
        colliderOffset = {offsetX, offsetY};
        colliderSize = {width, height};
    }

    /**
     * @brief Access the current primary animation.
     *
     * @return const Animation2D* Pointer to the animation or nullptr.
     */
    const Animation2D* GetCurrentAnimation() const { return currentAnimation.get(); }

    void SetCurrentAnimation(std::shared_ptr<Animation2D> anim) { currentAnimation = anim; }

    GameTypes::Direction GetFacingDirection() const noexcept { return facingDirection; }
    void SetFacingDirection(GameTypes::Direction dir) noexcept { facingDirection = dir; }

    void ResetToDefaultAnimation() { currentAnimation = defaultAnimation; }

    const GameLevel& GetGameLevel() const { return gameLevel; }

    /** Base implementation of Update method
     * Derived classes can override this to implement specific behavior
     */
    virtual void Update(float delta) {
        if (currentAnimation) {
            currentAnimation->Update(delta);
        }
    };

    // Draw the actor
    virtual void Draw() {
        if (currentAnimation) {
            currentAnimation->Draw(position, facingDirection == GameTypes::Direction::Left);
        }
    }

protected:
    Vector2 position;
    // Fixed physics collider (optional). When width/height > 0, used for all physics queries.
    Vector2 colliderOffset{0.0f, 0.0f};
    Vector2 colliderSize{0.0f, 0.0f};
    std::shared_ptr<Animation2D> defaultAnimation;  // default animation (when in default or idle state)
    std::shared_ptr<Animation2D> currentAnimation;  // current animation to be drawn
    GameLevel& gameLevel;                           // non-owning reference to the current game level
    bool alive = true;                  // flag to indicate if the actor is still alive (meaning not destroyed)
    ActorState actorState = STATE_IDLE; /**< Current runtime state */
    GameTypes::Direction facingDirection = GameTypes::Direction::Right; /**< Current facing direction */
};