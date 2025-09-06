#include "jump.h"
#include "canJump.h"
#include "gamelogic.h"

void Jump::Perform(float delta) {
    // Ensure target can jump (is of type CanJump - extends Movable)
    try {
        CanJump& canJumpActor = dynamic_cast<CanJump&>(actor);
        float jumpForce = (customJumpStrength > 0.0f) ? customJumpStrength : canJumpActor.GetJumpStrength();

        canJumpActor.DoJump();
        performedOnce = true; // mark that the oneShot action has been performed
    } catch (const std::bad_cast& e) {
        // Target actor does not support jumping; do nothing
        return;
    }
}