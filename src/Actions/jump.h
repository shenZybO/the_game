#pragma once

#include "action.h"

/**
 * @brief One-shot action that triggers a jump on a Jumpable actor.
 *
 * Executes once and then expires; uses the Jumpable interface to apply an
 * upward impulse.
 */
class Jump : public Action {
   public:
    /**
     * @brief Construct a Jump action.
     *
     * @param target Actor to jump (expected to be Jumpable).
     * @param customJumpStrength Optional jump strength override (0 uses Jumpable::GetJumpStrength()).
     */
    Jump(Actor& target, float customJumpStrength = 0.0f)
        : Action(target, 0.0f, true), customJumpStrength(customJumpStrength) {}

    ~Jump() = default;

    /**
     * @brief Perform the jump impulse when executed.
     *
     * @param delta Time in seconds since last frame.
     */
    void OnPerform(float delta) override;

   private:
    float customJumpStrength;  /**< Optional override for jump strength. */
};