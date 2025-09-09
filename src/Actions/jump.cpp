#include "jump.h"
#include "gamelogic.h"
#include "jumpable.h"

/**
 * @brief Perform the jump action on the target actor when possible.
 *
 * Uses a pointer-style dynamic_cast to safely detect whether the target supports Jumpable
 * behavior. If a custom strength was provided it is used; otherwise the Jumpable's configured
 * jump strength is used. The computed force is passed to DoJump.
 */
void Jump::OnPerform(float delta) {
    // Use pointer dynamic_cast to avoid exceptions when RTTI is enabled/disabled.
    Jumpable* jumpable = dynamic_cast<Jumpable*>(&actor);
    if (jumpable == nullptr) {
        return;
    }

    if (!jumpable->CanJump()) {
        return;
    }

    // Compute effective jump force and invoke the jump implementation.
    const float jumpForce = (customJumpStrength > 0.0f) ? customJumpStrength : jumpable->GetJumpStrength();
    jumpable->DoJump(jumpForce);
}