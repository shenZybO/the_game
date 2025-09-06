#include "canJump.h"

void CanJump::DoJump(float customStrength) {
    // wantsToJump = true;
    float jumpStrength = (customStrength > 0.0f) ? customStrength : this->jumpStrength;
    velocity.y = -jumpStrength;
    //SetGrounded(false);  // mark as not grounded when jump is initiated
    self.SetState(Actor::STATE_JUMPING);
}