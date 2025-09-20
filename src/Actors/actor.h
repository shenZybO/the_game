#pragma once

#include "types.h"
#include "raytmx.h"
#include "animation2d.h"
#include "animation2d_blinker.h"
#include "ianimation2d.h"
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
     * @brief General (non-movement) runtime states for an actor.
     *
     * Movement-specific states are now tracked separately by Movable.
     */
    enum ActorState {
        STATE_NORMAL = 0,     /// normal gameplay state
        STATE_TAKING_DAMAGE,  /// decreases lives (e.g. contact with enemy), actor remains in level if lives > 0
        STATE_DYING  /// actor is going to be removed e.g. fall to death (in case of Player this means level restart or
                     /// game over)
    };

    // Default ctor - keeps compatibility with existing code that constructs Actor without params
    Actor() = default;

    // Construct with an owned Animation2D (takes ownership)
    Actor(GameLevel& level, std::shared_ptr<Animation2D> anim, float x = 0.0f, float y = 0.0f)
        : position{x, y}, defaultAnimation(std::move(anim)), currentAnimation(defaultAnimation), gameLevel(level) {}

    // Construct by providing animation parameters; Actor will create its own Animation2D
    Actor(GameLevel& level, GameTypes::AnimationData idleAnim, float x = 0.0f, float y = 0.0f)
        : position{x, y}, gameLevel(level) {
        defaultAnimation = std::make_shared<Animation2D>(idleAnim);
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
     * @return std::shared_ptr<IAnimation2D> Pointer to the current animation (can be nullptr).
     */
    std::shared_ptr<IAnimation2D> GetCurrentAnimation() const { return currentAnimation; }

    /**
     * @brief Change the current animation.
     *
     * The provided animation becomes the current animation through shared ownership.
     * To revert to the default animation, call ResetToDefaultAnimation().
     */
    virtual void SetCurrentAnimation(std::shared_ptr<IAnimation2D> anim) {
        if (anim) {
            currentAnimation = anim;
        }
    }

    /**
     * @brief Get the current facing direction of the actor.
     *
     * @return GameTypes::Direction Current facing direction.
     */
    GameTypes::Direction GetFacingDirection() const noexcept { return facingDirection; }

    /**
     * @brief Set the current facing direction of the actor.
     */
    void SetFacingDirection(GameTypes::Direction dir) noexcept { facingDirection = dir; }

    /**
     * @brief Revert to the default/base animation.
     */
    void ResetToDefaultAnimation() { currentAnimation = defaultAnimation; }

    /**
     * @brief Access the game level the actor belongs to.
     *
     * @return const GameLevel& Reference to the current game level.
     */
    const GameLevel& GetGameLevel() const { return gameLevel; }

    /**
     * @brief Base implementation of Update method
     *
     * Derived classes can override this to implement specific behavior
     */
    virtual void Update(float delta) {
        if (currentAnimation) {
            currentAnimation->Update(delta);
        }
    };

    /**
     * @brief Draw the actor.
     *
     * This function is responsible for rendering the actor on the screen.
     */
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
    std::shared_ptr<IAnimation2D> defaultAnimation;  // default/base animation
    std::shared_ptr<IAnimation2D> currentAnimation;  // current animation to be drawn (can be decorator)
    GameLevel& gameLevel;                            // non-owning reference to the current game level
    bool alive = true;                    // flag to indicate if the actor is still alive (meaning not destroyed)
    ActorState actorState = STATE_NORMAL; /**< Current general runtime state */
    GameTypes::Direction facingDirection = GameTypes::Direction::Right; /**< Current facing direction */
};