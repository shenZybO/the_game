#pragma once

#include "types.h"
#include "movable.h"
#include "action.h"
#include "animation2d.h"
#include <memory>

class Jumpable : virtual public Movable {
    public:
        Jumpable(float jumpStrength) 
            : Movable(*this), jumpStrength(jumpStrength) {}

        Jumpable(float jumpStrength, GameTypes::AnimationData jumpingAnim)
            : Movable(*this), jumpStrength(jumpStrength), 
            jumpingAnimation(std::make_shared<Animation2D>(jumpingAnim)) {}

        ~Jumpable() override = default;
        
        void DoJump( float jumpStrength );
        bool CanJump() const noexcept;
        void ResetJump() { doubleJumpDone = false; }
        float GetJumpStrength() const noexcept { return jumpStrength; }

        void Update(float delta);

    protected:

    private:
        float jumpStrength; /**< Jump impulse strength. */
        bool doubleJumpDone = false; /**< Tracks if the double jump has been performed */
        std::shared_ptr<Animation2D> jumpingAnimation; /**< Optional jumping animation; falls back to defaultAnimation */
};