#include "player.h"
#include "raytmx.h"
#include <cstring>
#include "move.h"
#include "gamelogic.h"
#include "gamelevel.h"

#define GROUND_LAYER_NAME "ground"

bool wantsToJump = false;  // TODO: add this in can jump interface

// Destructor is defined inline in header (unregisters input listener)

void Player::Draw() {
    // Determine facing based on horizontal velocity: when moving left, flip the sprite
    bool facingLeft = (velocity.x < 0);

    // Use the base class animation to draw at the player's position
    if (defaultAnimation) {
        // Draw takes position, tint, scale, flipped
        defaultAnimation->Draw(position, WHITE, 1.0f, facingLeft);
    }
}

void Player::Update(float delta) {
    // Movable::Update handles animation, grounded state and gravity
    Movable::Update(delta);

    // Gravity
    if (!isGrounded || wantsToJump) {
        // Apply gravity only if not grounded
        velocity.y += 800.0f * delta;  // gravity constant
        wantsToJump = false;           // reset jump request after applying jump
        // Update vertical position
        position.y += velocity.y * delta;
    } else {
        velocity.y = 0;  // reset vertical velocity when grounded
    }
}

void Player::OnKeyPressed(int key) {
    if ((key == KEY_LEFT || key == KEY_RIGHT)) {
        Move::Direction dir = (key == KEY_LEFT) ? Move::Direction::Left : Move::Direction::Right;
        auto act = std::make_unique<Move>(*this, dir);
        Action* raw = act.get();
        // if there is an active move action, deregister it first
        if (activeMoveAction) {
            GameLogic::Instance().DeregisterAction(activeMoveAction);
            activeMoveAction = nullptr;
        }
        GameLogic::Instance().RegisterAction(std::move(act));
        activeMoveAction = raw;
    }

    if (key == KEY_SPACE && isGrounded) {
        velocity.y = -jumpStrength;
        wantsToJump = true;
    }
}

void Player::OnKeyReleased(int key) {
    if ((key == KEY_LEFT || key == KEY_RIGHT) && activeMoveAction) {
        // only deregister if key released matches the active move action direction
        if ((key == KEY_LEFT && IsMovingLeft()) || (key == KEY_RIGHT && IsMovingRight())) {
            GameLogic::Instance().DeregisterAction(activeMoveAction);
            activeMoveAction = nullptr;
        }
    }
}
