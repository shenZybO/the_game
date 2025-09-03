#include "move.h"
#include "player.h"

void Move::Perform(float delta) {
    // Ensure target is a Movable
    Movable& movableActor = dynamic_cast<Movable&>(GetActor());

    float speed = (customSpeed > 0.0f) ? customSpeed : movableActor.GetMoveSpeed();

    float displacement = speed * delta;
    if (moveDir == Direction::Left) {
        movableActor.MoveBy(-displacement, 0.0f);
        // keep horizontal velocity in sync for drawing/facing logic
        movableActor.SetVelocityX(-speed);
        movableActor.SetState(Actor::STATE_MOVING_LEFT);
    } else {
        movableActor.MoveBy(displacement, 0.0f);
        // keep horizontal velocity in sync for drawing/facing logic
        movableActor.SetVelocityX(speed);
        movableActor.SetState(Actor::STATE_MOVING_RIGHT);
    }
}

Move::~Move() {
    Movable& movableActor = dynamic_cast<Movable&>(GetActor());
    movableActor.SetVelocityX(0.0f);
    movableActor.SetState(Actor::STATE_IDLE);
}
