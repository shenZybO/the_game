#pragma once

#include "types.h"
#include "movable.h"
#include <random>
#include <memory>

class Patrolable : virtual public Movable {
    public:
        enum class PatrolState { FallingToGround, Moving, Waiting };
        
        Patrolable() 
            : Movable(*this),
            rng(std::random_device{}()) {}

        ~Patrolable() override = default;
        
        void Update(float delta);

    protected:

    private:
        PatrolState state = PatrolState::FallingToGround;
        float waitTimer = 0.0f;
        std::mt19937 rng{std::random_device{}()};
        GameTypes::Direction patrolDir = GameTypes::Direction::Right;
};