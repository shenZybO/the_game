#include "jump.h"
#include "gamelogic.h"
#include "jumpable.h"

void Jump::OnPerform(float delta) {
    // Ensure target is a Jumpable
    if (auto* jumpable = dynamic_cast<Jumpable*>(&actor)) {
        if (jumpable->CanJump()) {
           const float jumpForce = (customJumpStrength > 0.0f)
               ? customJumpStrength
               : jumpable->GetJumpStrength();
           jumpable->DoJump(jumpForce);
       }
   }
}