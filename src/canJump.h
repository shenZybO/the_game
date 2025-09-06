#pragma once

#include "movable.h"
#include "action.h"

class CanJump : virtual public Movable {
    public:
        CanJump(float jumpStrength) 
            : Movable(*this), jumpStrength(jumpStrength) {}

        virtual ~CanJump() = default;
        virtual void DoJump(float customStrength = 0.0f);

        float GetJumpStrength() const { return jumpStrength; }

    protected:
        Action* activeJumpAction = nullptr; /**< Currently active Move action, if any. */

    private:
        //bool wantsToJump = false;  // marker for intention to jump
        float jumpStrength; /**< Jump impulse strength. */

};