#pragma once

#include "types.h"
#include "actor.h"
#include "action.h"
#include "raylib.h"

/**
 * @brief Ability mixin adding movement and basic physics to an Actor.
 *
 * Movable provides velocity, gravity handling, ground-snapping and optional
 * animations for moving/falling. It is designed as a mixin and holds a
 * non-owning reference to the `Actor` it augments.
 */
class Movable /* : public Actor */ {
   public:
    /**
     * @brief Construct a Movable actor with optional default animation.
     *
     * @param moveSpeed Horizontal movement speed.
     */
    Movable(Actor& self, float moveSpeed = 0.0f)
        : self(self),
          velocity{0, 0},
          moveSpeed(moveSpeed),
          isGrounded(false) {}

  /**
   * @brief Construct a Movable and provide a separate moving animation.
   *
   * If movingImagePath is null or empty, the actor's default animation will be used for moving.
   */
  Movable(Actor& self, GameTypes::AnimationData moveAnim, float moveSpeed = 0.0f)
    : self(self),
      velocity{0, 0},
      moveSpeed(moveSpeed),
      isGrounded(false),
      movingAnimation(std::make_shared<Animation2D>(moveAnim)) {}

  Movable(Actor& self, GameTypes::AnimationData moveAnim, GameTypes::AnimationData fallAnim, float moveSpeed = 0.0f)
    : self(self),
      velocity{0, 0},
      moveSpeed(moveSpeed),
      isGrounded(false),
      movingAnimation(std::make_shared<Animation2D>(moveAnim)),
      fallingAnimation(std::make_shared<Animation2D>(fallAnim)) {}

  /**
   * @brief Virtual destructor.
   */
  virtual ~Movable() = default;

  /**
   * @brief Get current velocity vector (pixels per second).
   */
  Vector2 GetVelocity() const { return velocity; }

  /**
   * @brief Set horizontal velocity component.
   */
  void SetVelocityX(float vx) { velocity.x = vx; }

  /**
   * @brief Set vertical velocity component.
   */
  void SetVelocityY(float vy) { velocity.y = vy; }

  /**
   * @brief Get configured horizontal move speed.
   */
  float GetMoveSpeed() const { return moveSpeed; }

  /**
   * @brief Set the horizontal move speed.
   */
  void SetMoveSpeed(float s) { moveSpeed = s; }

  /**
   * @brief Query whether the actor is currently grounded.
   */
  bool IsGrounded() const { return isGrounded; }

  /**
   * @brief Query whether actor is moving left.
   */
  bool IsMovingLeft() const { return velocity.x < 0; }

  /**
   * @brief Query whether actor is moving right.
   */
  bool IsMovingRight() const { return velocity.x > 0; }

  /**
   * @brief Set a separate moving animation instance (takes ownership).
   *
   * @param anim Unique pointer to an Animation2D instance.
   */
  void SetMovingAnimation(std::unique_ptr<Animation2D> anim) { movingAnimation = std::move(anim); }

  /**
   * @brief Move the actor by the given delta in world units.
   *
   * @param dx Horizontal delta.
   * @param dy Vertical delta.
   */
  void MoveBy(float dx, float dy);

  void Update(float delta);

  protected:
    Vector2 velocity{0, 0};
    bool isGrounded = false;
    Action* activeMoveAction = nullptr; /**< Currently active Move action, if any. */
    Actor& self; // non owning actor reference

    // Helper: return true when tile exists under the tile at the given world coordinates
    bool HasGroundTileAt(float worldX, float worldY) const;

  private:
    Vector2 prevPosition;
    float moveSpeed;
    std::shared_ptr<Animation2D> movingAnimation; /**< Optional moving animation; falls back to defaultAnimation */
    std::shared_ptr<Animation2D> fallingAnimation; /**< Optional falling animation; falls back to defaultAnimation */
    // Check if this movable is on the ground and correct position if needed
    bool CheckAndFixGroundCollision(float delta);

};
