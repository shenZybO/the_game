#include "move.h"
#include "movable.h"

/**
 * @brief Apply horizontal movement each frame for Move action.
 *
 * This dynamically checks that the target supports `Movable` and then moves
 * it by a delta computed from configured speed and the frame delta.
 */
void Move::OnPerform(float delta) {
    // Ensure target is a Movable
    Movable* movableActor = dynamic_cast<Movable*>(&actor);
    if (!movableActor) {
        return;
    }
    float speed = (customSpeed > 0.0f) ? customSpeed : movableActor->GetMoveSpeed();

    float displacement = speed * delta;
    switch (moveDir) {
        case GameTypes::Direction::Left:
            movableActor->MoveBy(-displacement, 0.0f);
            // keep horizontal velocity in sync for drawing/facing logic
            movableActor->SetVelocityX(-speed);
            break;
        case GameTypes::Direction::Right:
            movableActor->MoveBy(displacement, 0.0f);
            // keep horizontal velocity in sync for drawing/facing logic
            movableActor->SetVelocityX(speed);
            break;
        default:
            movableActor->SetVelocityX(0.0f);
            return;  // invalid direction
    }
}

/**
 * @brief Destructor ensures velocity is reset when action is destroyed.
 */
Move::~Move() {
    Movable& movableActor = dynamic_cast<Movable&>(actor);
    movableActor.SetVelocityX(0.0f);
}
