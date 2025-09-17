#pragma once

#include "raylib.h"
class Actor;

/**
 * @brief Interface for objects that wish to receive collision notifications.
 *
 * Implementors must return a reference to the underlying `Actor` that
 * provides the collision bounds and will be reported as `self` during
 * notifications.
 */
class ICollisionListener {
public:
    virtual ~ICollisionListener() = default;

    /**
     * @brief Return the underlying actor used for collision bounds.
     */
    virtual Actor& GetCollisionActor() = 0;

    /**
     * @brief Called when this listener's actor collides with another actor.
     * @param self Reference to the listener actor (same as GetCollisionActor()).
     * @param other Reference to the other actor involved in the collision.
     * @param overlap Overlapping rectangle (intersection of AABBs).
     */
    virtual void OnCollision(Actor& self, Actor& other, const Rectangle& overlap) = 0;
};
