#include "jumpable.h"
#include "config.hpp"

void Jumpable::DoJump(float jumpStrength) {
    velocity.y = -jumpStrength;
    self.SetState(Actor::STATE_JUMPING);
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

    if (self.GetState() == Actor::STATE_JUMPING) {
        if (jumpingAnimation) {
            self.SetCurrentAnimation(jumpingAnimation);
        }
    }
}