#pragma once

#include "types.h"
#include "movable.h"
#include "action.h"
#include "animation2d.h"
#include <memory>

/**
 * @brief Ability mixin enabling jump behavior for an Actor.
 *
 * Provides jump strength, double-jump tracking and optional jump animation.
 */
class Jumpable : virtual public Movable {
    public:
        /**
         * @brief Construct a Jumpable mixin with a configured jump strength.
         *
         * @param jumpStrength Strength of jump impulse in world units.
         */
        Jumpable(float jumpStrength) 
            : Movable(*this), jumpStrength(jumpStrength) {}

        /**
         * @brief Construct a Jumpable mixin with an associated jump animation.
         *
         * @param jumpStrength Strength of jump impulse in world units.
         * @param jumpingAnim Animation data for jump state.
         */
        Jumpable(float jumpStrength, GameTypes::AnimationData jumpingAnim)
            : Movable(*this), jumpStrength(jumpStrength), 
            jumpingAnimation(std::make_shared<Animation2D>(jumpingAnim)) {}

        /**
         * @brief Virtual destructor.
         */
        ~Jumpable() override = default;
        
        /**
         * @brief Apply a jump impulse to the actor.
         *
         * @param jumpStrength Upward impulse strength to apply (positive value).
         */
        void DoJump(float jumpStrength);

        /**
         * @brief Query whether the actor may perform a jump now.
         *
         * Returns true when the actor is grounded, or if a double-jump is
         * allowed and hasn't yet been performed.
         *
         * @return true if a jump can be initiated.
         */
        bool CanJump() const noexcept;

        /**
         * @brief Reset internal double-jump tracking (used when actor lands).
         */
        void ResetJump() { doubleJumpDone = false; }

        /**
         * @brief Get the configured jump strength for this Jumpable.
         *
         * @return float Configured jump impulse strength.
         */
        float GetJumpStrength() const noexcept { return jumpStrength; }

        /**
         * @brief Per-frame update to manage jump-related state and animation.
         *
         * @param delta Time in seconds since last frame.
         */
        void Update(float delta);

    protected:

    private:
        float jumpStrength; /**< Jump impulse strength. */
        bool doubleJumpDone = false; /**< Tracks if the double jump has been performed */
        std::shared_ptr<Animation2D> jumpingAnimation; /**< Optional jumping animation; falls back to defaultAnimation */
};