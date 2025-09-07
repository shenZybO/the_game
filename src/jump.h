#pragma once

#include "action.h"

/* Jump action */
class Jump : public Action {
   public:
    Jump(Actor& target, float customJumpStrength = 0.0f)
        : Action(target, 0.0f, true), customJumpStrength(customJumpStrength) {}

    ~Jump() = default;

    void OnPerform(float delta) override;

   private:
    float customJumpStrength;  // optional override for move speed magnitude
};