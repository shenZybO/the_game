#pragma once

#include "actor.h"
#include "raylib.h"

/* Abstract base for actors that can move. Extracted from Player: velocity, moveSpeed and
 * isGrounded. */
class Movable : public Actor {
   public:
    Movable(GameLevel& level, const char* imagePath = nullptr, int frameCount = 1,
            float frameDuration = 0.1f, float x = 0.0f, float y = 0.0f, float moveSpeed = 0.0f)
        : Actor(level, imagePath ? imagePath : "", frameCount, frameDuration, x, y),
          velocity{0, 0},
          moveSpeed(moveSpeed),
          isGrounded(false) {}

    virtual ~Movable() = default;

    Vector2 GetVelocity() const { return velocity; }
    void SetVelocityX(float vx) { velocity.x = vx; }
    void SetVelocityY(float vy) { velocity.y = vy; }

    float GetMoveSpeed() const { return moveSpeed; }
    void SetMoveSpeed(float s) { moveSpeed = s; }

    void SetGrounded(bool g) { isGrounded = g; }
    bool IsGrounded() const { return isGrounded; }

    bool IsMovingLeft() const { return velocity.x < 0; }
    bool IsMovingRight() const { return velocity.x > 0; }

    // Move the actor by the given delta in world units
    void MoveBy(float dx, float dy);

   protected:
    Vector2 velocity;
    float moveSpeed;
    bool isGrounded;
};
