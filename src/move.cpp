#include "move.h"
#include "movable.h"

void Move::Perform(float delta) {
    // Ensure target is a Movable
    try {
        Movable& movableActor = dynamic_cast<Movable&>(actor);
        float speed = (customSpeed > 0.0f) ? customSpeed : movableActor.GetMoveSpeed();

        float displacement = speed * delta;
        if (moveDir == Direction::Left) {
            movableActor.MoveBy(-displacement, 0.0f);
            // keep horizontal velocity in sync for drawing/facing logic
            movableActor.SetVelocityX(-speed);
            actor.SetState(Actor::STATE_MOVING_LEFT);
        } else {
            movableActor.MoveBy(displacement, 0.0f);
            // keep horizontal velocity in sync for drawing/facing logic
            movableActor.SetVelocityX(speed);
            actor.SetState(Actor::STATE_MOVING_RIGHT);
        }
    } catch (const std::bad_cast& e) {
        // Target actor is not Movable; do nothing
        return;
    }
}

Move::~Move() {
    Movable& movableActor = dynamic_cast<Movable&>(actor);
    movableActor.SetVelocityX(0.0f);
}
