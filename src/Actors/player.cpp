#include "player.h"
#include "raytmx.h"
#include <cstring>
#include "move.h"
#include "gamelogic.h"
#include "gamelevel.h"
#include "jump.h"
#include "types.h"

// Destructor is defined inline in header (unregisters input listener)

/**
 * @brief Draw the player using Actor drawing logic.
 */
void Player::Draw() {
    Actor::Draw();
}

/**
 * @brief Per-frame update for the player.
 *
 * This updates animations, jump state and physics integration in the proper order.
 */
void Player::Update(float delta) {
    // Update base animation state first
    Actor::Update(delta);
    // Update jump-related state (resets double-jump when grounded, sets jump animation)
    Jumpable::Update(delta);
    // Then apply physics/movement integration
    Movable::Update(delta);
}

/**
 * @brief Handle key press events for movement and jumping.
 *
 * Registers Move/Jump actions in the global GameLogic when appropriate keys are pressed.
 */
void Player::OnKeyPressed(int key) {
    if ((key == KEY_LEFT || key == KEY_RIGHT)) {
        GameTypes::Direction dir = (key == KEY_LEFT) ? GameTypes::Direction::Left : GameTypes::Direction::Right;
        auto act = std::make_unique<Move>(*this, dir);
        Action* raw = act.get();
        // If a previous move action exists, remove it before registering a new one
        if (activeMoveAction) {
            GameLogic::Instance().DeregisterAction(activeMoveAction);
            activeMoveAction = nullptr;
        }
        GameLogic::Instance().RegisterAction(std::move(act));
        activeMoveAction = raw;
    }

    if (key == KEY_SPACE && CanJump()) {
        auto act = std::make_unique<Jump>(*this);
        GameLogic::Instance().RegisterAction(std::move(act));
    }
}

/**
 * @brief Handle key release events; stop movement if the released key matches current movement.
 */
void Player::OnKeyReleased(int key) {
    if ((key == KEY_LEFT || key == KEY_RIGHT) && activeMoveAction) {
        // Only deregister when the released key corresponds to current motion direction
        if ((key == KEY_LEFT && IsMovingLeft()) || (key == KEY_RIGHT && IsMovingRight())) {
            GameLogic::Instance().DeregisterAction(activeMoveAction);
            activeMoveAction = nullptr;
        }
    }
}
