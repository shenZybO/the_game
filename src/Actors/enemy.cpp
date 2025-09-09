#include "enemy.h"

void Enemy::Update(float delta) {
    // Call base updates to apply gravity and animations
    Actor::Update(delta);
    Movable::Update(delta);
    Patrolable::Update(delta);
}

void Enemy::Draw() {
    // call Actor::Draw for default drawing behavior
    Actor::Draw();
}
