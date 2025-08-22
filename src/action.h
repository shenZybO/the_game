#pragma once

#include "actor.h"
#include <cstdint>

/* Abstract representation of an action that can be performed on an Actor. */
class Action {
public:
    // durationSeconds == 0 -> not timed (performed indefinitely until deregistered)
    Action(Actor& target, float durationSeconds = 0.0f) : actor(target), duration(durationSeconds), elapsed(0.0f) {}
    virtual ~Action() = default;

    // Called each update tick. Implementations perform their logic.
    virtual void Perform(float delta) = 0;

    Actor& GetActor() const { return actor; }

    bool IsTimed() const { return duration > 0.0f; }
    // Advance internal timer; call after Perform
    void AdvanceTime(float delta) { if (IsTimed()) elapsed += delta; }
    bool IsExpired() const { return IsTimed() && (elapsed >= duration); }

protected:
    Actor& actor; // non-owning reference to the actor on which the action will be performed
    float duration; // seconds; 0 means infinite
    float elapsed; // seconds
};
