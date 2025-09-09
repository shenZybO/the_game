#pragma once

#include "actor.h"
#include "patrolable.h"
#include "movable.h"
#include "types.h"
#include <random>

/**
 * Simple Enemy actor: falls until it lands on ground, then patrols horizontally between ground edges.
 */
class Enemy : public Actor, virtual public Movable, public Patrolable {
  public:
    enum class PatrolState { FallingToGround, Moving, Waiting };

    Enemy(GameLevel& level, float x, float y,
           float moveSpeed, GameTypes::AnimationData idleAnim, GameTypes::AnimationData patrolAnim)
        : Actor(level, idleAnim, x, y),
          Movable(*this, patrolAnim, moveSpeed),
          Patrolable() {}

    void Update(float delta) override;

    void Draw() override;

  private:
};
