#include "jumpable.h"
#include "config.hpp"

void Jumpable::DoJump(float jumpStrength) {
    velocity.y = -jumpStrength;
    SetMovementState(Movable::MovementState::Jumping);
    if (!isGrounded) {
        doubleJumpDone = true;
    }
}

bool Jumpable::CanJump() const noexcept {
    return isGrounded || (!doubleJumpDone && PlayerConfig::CAN_DOUBLE_JUMP);
}

void Jumpable::Update(float delta) {
    if (isGrounded) {
        doubleJumpDone = false;  // reset jump state when grounded
    }

    if (GetPreviousMovementState() != Movable::MovementState::Jumping &&
        GetMovementState() == Movable::MovementState::Jumping) {
        if (jumpingAnimation) {
            self.SetCurrentAnimation(jumpingAnimation);
        }
    }
}