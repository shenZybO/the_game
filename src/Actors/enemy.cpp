#include "enemy.h"
#include "config.hpp"

/**
 * @brief Update the enemy each frame.
 *
 * This forwards to base implementations to apply animation updates, physics
 * and patrol behaviour supplied by the mixins.
 *
 * @param delta Time in seconds since last frame.
 */
void Enemy::Update(float delta) {
    /* Call the base and mixin update routines in the correct order so that
       animation advancement, physics integration and patrol decisions are
       performed every frame. */
    Actor::Update(delta);
    Movable::Update(delta);
    Patrolable::Update(delta);
}

/**
 * @brief Draw the enemy using current animation frame.
 */
void Enemy::Draw() {
    /* Use Actor::Draw which handles animation drawing and flipping based on
       facing direction. No additional drawing layers are needed here. */
    Actor::Draw();
}

void Enemy::EnemyInit() {
    /*
     * Configure a fixed physics collider for enemies.
     */
    SetCollider(EnemyConfig::COLLIDER_OFFSET_X,
          EnemyConfig::COLLIDER_OFFSET_Y,
          EnemyConfig::COLLIDER_WIDTH,
          EnemyConfig::COLLIDER_HEIGHT);
}
