#pragma once

#include "actor.h"
#include <cstdint>

/**
 * @brief Abstract representation of an action that can be performed on an Actor.
 *
 * Actions encapsulate repeated or timed behavior that the game logic executes each frame.
 * Ownership of `Action` instances is transferred to [`GameLogic`](src/Logic/gamelogic.h).
 */
class Action {
   public:
    /**
     * @brief Construct an Action.
     *
     * @param target Actor on which this action will operate (non-owning reference).
     * @param durationSeconds Duration in seconds; zero means infinite until deregistered.
     * @param oneShot If true the action will be considered performed after first execution.
     */
    Action(Actor& target, float durationSeconds = 0.0f, bool oneShot = false)
        : actor(target), duration(durationSeconds), oneShot(oneShot), elapsed(0.0f) {}
    virtual ~Action() = default;

    /**
     * @brief Called each update tick. Implementations perform their logic.
     *
     * This method calls the concrete implementation `OnPerform` and updates internal performed flag
     * for one-shot actions. It intentionally does not advance the elapsed time; the caller should
     * call `AdvanceTime` after `Perform`.
     *
     * @param delta Time in seconds since last frame.
     */
    void Perform(float delta) {
        if (IsExpired()) {
            return; // already performed once, skip further calls
        }
        OnPerform(delta);
        performedOnce = true; // mark that the oneShot action has been performed
    }

    /**
     * @brief Access the target actor reference.
     *
     * @return Actor& Non-owning reference to actor.
     */
    Actor& GetActor() { return actor; }

    /**
     * @brief Returns true if this action uses a positive duration.
     */
    bool IsTimed() const { return duration > 0.0f; }

    /**
     * @brief Advance the internal timer; should be called after `Perform`.
     *
     * @param delta Seconds to advance.
     */
    void AdvanceTime(float delta) {
        if (IsTimed()) elapsed += delta;
    }

    /**
     * @brief Check if action duration has been exceeded or if a one-shot action was performed.
     *
     * @return true if action should be removed.
     */
    bool IsExpired() const { return (IsTimed() && (elapsed >= duration)) || (oneShot && performedOnce); }

   protected:
    Actor& actor;    /**< Non-owning reference to the actor target. */
    float duration;  /**< Seconds; 0 means infinite. */
    bool oneShot;    /**< If true, action is performed only once per registration. */
    bool performedOnce = false; /**< Tracks if one-shot action already ran. */
    float elapsed;   /**< Accumulated seconds. */

    /**
     * @brief Hook for derived implementations; called by `Perform`.
     *
     * Derived classes must implement this to provide action behavior.
     *
     * @param delta Time in seconds since last frame.
     */
    virtual void OnPerform(float delta) = 0;
};
