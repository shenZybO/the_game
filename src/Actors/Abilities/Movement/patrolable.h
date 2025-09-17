#pragma once

#include "types.h"
#include "movable.h"
#include <random>
#include <memory>

/**
 * @brief Mixin for simple patrol behaviour.
 *
 * Patrolable allows an actor to fall until it lands, then patrol left/right
 * between edges. It uses the Movable ability for movement and checks ground
 * tiles to determine edges. The class expects `self` to be a Movable/Actor.
 */
class Patrolable : virtual public Movable {
    public:
        enum class PatrolState { FallingToGround, Moving, Waiting };
        
        Patrolable() 
            : Movable(*this),
            rng(std::random_device{}()) {}

        ~Patrolable() override = default;
        
        /**
         * @brief Update patrol state and schedule/cleanup movement actions.
         *
         * @param delta Time in seconds since last frame.
         */
        void Update(float delta);

    protected:
    /**
     * @brief Reverse current patrol movement direction (if in Moving state).
     *
     * Deregisters the active move action so a new one for the opposite
     * direction can be scheduled on the next update tick. Used by the
     * collision system when encountering horizontal obstacles.
     */
        void ReversePatrolDirection();

    private:
        PatrolState state = PatrolState::FallingToGround;
        float waitTimer = 0.0f;
        std::mt19937 rng{std::random_device{}()};
        GameTypes::Direction patrolDir = GameTypes::Direction::Right;
};