#pragma once

#include "actor.h"
#include "patrolable.h"
#include "movable.h"
#include "types.h"
#include <random>

/**
 * @brief Simple Enemy actor: falls until it lands on ground, then patrols horizontally.
 *
 * The Enemy composes `Movable` and `Patrolable` to get gravity, collision and patrol behaviour.
 */
class Enemy : public Actor, virtual public Movable, public Patrolable {
  public:

    Enemy(GameLevel& level, float x, float y,
           float moveSpeed, GameTypes::AnimationData idleAnim, GameTypes::AnimationData patrolAnim)
        : Actor(level, idleAnim, x, y),
          Movable(*this, patrolAnim, moveSpeed),
          Patrolable() {}

    /**
     * @brief Update enemy each frame: animations, physics and patrol logic.
     *
     * @param delta Time in seconds since last frame.
     */
    void Update(float delta) override;

    /**
     * @brief Draw the enemy using the current animation frame.
     */
    void Draw() override;

  private:
};
