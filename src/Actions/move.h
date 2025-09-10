#pragma once

#include "types.h"
#include "action.h"
#include "movable.h"

/**
 * @brief Action that applies horizontal movement to a Movable actor.
 *
 * When registered with the `GameLogic`, this action moves the target each frame
 * in the specified direction until deregistered.
 */
class Move : public Action {
   public:
    /**
     * @brief Construct a Move action.
     *
     * @param target Actor to move (must be Movable).
     * @param dir Direction to move (Left/Right).
     * @param customSpeed Optional override for movement speed (0 uses Movable::GetMoveSpeed()).
     */
    Move(Actor& target, GameTypes::Direction dir, float customSpeed = 0.0f)
        : Action(target), moveDir(dir), customSpeed(customSpeed) {}

    ~Move();

    /**
     * @brief Called each frame to apply movement.
     *
     * @param delta Time in seconds since last frame.
     */
    void OnPerform(float delta) override;

   private:
    const GameTypes::Direction moveDir; /**< Direction of motion. */
    float customSpeed;                  /**< Optional speed override. */
};
