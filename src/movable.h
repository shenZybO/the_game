#pragma once

#include "actor.h"
#include "raylib.h"

/* Abstract base for actors that can move. Extracted from Player: velocity, moveSpeed and
 * isGrounded. */
class Movable : public Actor {
   public:
    /**
     * @brief Construct a Movable actor with optional default animation.
     *
     * @param level Owning GameLevel reference.
     * @param imagePath Path to default animation image (nullptr or empty allowed).
     * @param frameCount Number of frames in default animation.
     * @param frameDuration Seconds per frame.
     * @param x Initial X position.
     * @param y Initial Y position.
     * @param moveSpeed Horizontal movement speed.
     */
    Movable(GameLevel& level, const char* imagePath = nullptr, int frameCount = 1,
            float frameDuration = 0.1f, float x = 0.0f, float y = 0.0f, float moveSpeed = 0.0f)
        : Actor(level, imagePath ? imagePath : "", frameCount, frameDuration, x, y),
          velocity{0, 0},
          moveSpeed(moveSpeed),
          isGrounded(false) {}

  /**
   * @brief Construct a Movable and provide a separate moving animation.
   *
   * If movingImagePath is null or empty, the actor's default animation will be used for moving.
   */
  Movable(GameLevel& level, const char* imagePath, int frameCount, float frameDuration,
      const char* movingImagePath, int movingFrameCount, float movingFrameDuration,
      float x = 0.0f, float y = 0.0f, float moveSpeed = 0.0f)
    : Actor(level, imagePath, frameCount, frameDuration, x, y),
      velocity{0, 0},
      moveSpeed(moveSpeed),
      isGrounded(false) {
    if (movingImagePath != nullptr && movingImagePath[0] != '\0') {
      movingAnimation = std::make_unique<Animation2D>(movingImagePath, movingFrameCount, movingFrameDuration);
    }
  }

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
   * @brief Mark the actor as grounded or not.
   */
  void SetGrounded(bool g) { isGrounded = g; }

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

  /**
   * @brief Per-frame update: update animations, apply gravity and resolve collisions.
   *
   * @param delta Seconds since last update.
   */
  void Update(float delta) override;

  /**
   * @brief Draw override to use movement-specific animation when moving.
   */
  void Draw() override;

   private:
    // Check if this movable is on the ground and correct position if needed
    bool CheckAndFixGroundCollision(float delta);

   protected:
    Vector2 prevPosition;
    Vector2 velocity;
    float moveSpeed;
    bool isGrounded;
    std::unique_ptr<Animation2D> movingAnimation; /**< Optional moving animation; falls back to defaultAnimation */
    bool HasMovingAnimation() const { return movingAnimation != nullptr; }
};
