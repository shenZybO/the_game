#include "player.h"
#include "raytmx.h"
#include <cstring>
#include "move.h"
#include "gamelogic.h"
#include "gamelevel.h"
#include "jump.h"
#include "types.h"

// Destructor is defined inline in header (unregisters input listener)

void Player::Draw() {
    // call Actor::Draw for default drawing behavior
    Actor::Draw();
}

void Player::Update(float delta) {
    // Actor::Update for default animation update
    Actor::Update(delta);
    // Update jump state/vertical forces before generic movement integration
    Jumpable::Update(delta);
    Movable::Update(delta);
}

void Player::OnKeyPressed(int key) {
    if ((key == KEY_LEFT || key == KEY_RIGHT)) {
        GameTypes::Direction dir = (key == KEY_LEFT) ? GameTypes::Direction::Left : GameTypes::Direction::Right;
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

    if (key == KEY_SPACE && CanJump()) {
        auto act = std::make_unique<Jump>(*this);
        GameLogic::Instance().RegisterAction(std::move(act));
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
