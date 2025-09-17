#pragma once

#include <vector>
#include <unordered_set>
#include <utility>
#include "collision_listener.h"
#include "actor.h"

/**
 * @brief Basic axis-aligned bounding box (AABB) collision system.
 *
 * The system tests all registered collision listener actors against the
 * complete set of actors in the current level (including the player). On
 * overlap, it invokes `OnCollision` providing both actors and the overlap
 * rectangle. It performs a naive O(N*M) broad-phase suitable for the small
 * actor counts typical in early prototypes.
 */
class CollisionSystem {
public:
    static CollisionSystem& Instance();

    /**
     * @brief Register a collision listener (no ownership transfer).
     */
    void RegisterListener(ICollisionListener* listener);

    /**
     * @brief Unregister a previously registered listener.
     */
    void UnregisterListener(ICollisionListener* listener);

    /**
     * @brief Run collision detection between registered listeners and all actors in level.
     */
    void Update(const std::vector<std::unique_ptr<Actor>>& actors, Actor* player);

private:
    std::vector<ICollisionListener*> listeners;
};
