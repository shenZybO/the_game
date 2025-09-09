#pragma once

#include "actor.h"
#include <cstdint>

/* Abstract representation of an action that can be performed on an Actor. */
class Action {
   public:
    // durationSeconds == 0 -> not timed (performed indefinitely until deregistered)
    Action(Actor& target, float durationSeconds = 0.0f, bool oneShot = false)
        : actor(target), duration(durationSeconds), oneShot(oneShot), elapsed(0.0f) {}
    virtual ~Action() = default;

    // Called each update tick. Implementations perform their logic.
    void Perform(float delta) {
        if (IsExpired()) {
            return; // already performed once, skip further calls
        }
        // Call the derived class implementation
        OnPerform(delta);
        performedOnce = true; // mark that the oneShot action has been performed
    }

    Actor& GetActor() { return actor; }

    bool IsTimed() const { return duration > 0.0f; }
    // Advance internal timer; call after Perform
    void AdvanceTime(float delta) {
        if (IsTimed()) elapsed += delta;
    }
    // Check if action duration has been exceeded
    bool IsExpired() const { return (IsTimed() && (elapsed >= duration)) || (oneShot && performedOnce); }

   protected:
    Actor& actor;    // non-owning reference to the actor on which the action will be performed
    float duration;  // seconds; 0 means infinite
    bool oneShot;   // if true, action is performed only once per registration
    bool performedOnce = false; // internal flag to track if oneShot action has been performed at least once
    float elapsed;   // seconds

    // Hook for derived implementations; called by Perform().
    virtual void OnPerform(float delta) = 0; // must be implemented by derived classes
};
