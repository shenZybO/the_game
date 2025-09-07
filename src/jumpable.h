#pragma once

#include "movable.h"
#include "action.h"

class Jumpable : virtual public Movable {
    public:
        Jumpable(float jumpStrength) 
            : Movable(*this), jumpStrength(jumpStrength) {}

        Jumpable(float jumpStrength, const char* jumpingImagePath, int jumpingFrameCount, float jumpingFrameDuration) 
            : Movable(*this), jumpStrength(jumpStrength) {
            if (jumpingImagePath != nullptr && jumpingImagePath[0] != '\0') {
                jumpingAnimation = std::make_shared<Animation2D>(jumpingImagePath, jumpingFrameCount, jumpingFrameDuration);
            }
        }

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