#include "player.h"
#include "raytmx.h"
#include <cstring>
#include "move.h"
#include "gamelogic.h"
#include "gamelevel.h"

#define GROUND_LAYER_NAME "ground"

TmxObject tmxCollisionObject = {};  // TODO: resolve colision detection with ground layer
bool wantsToJump = false;           // TODO: add this in can jump interface

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
    // call base class Update
    Actor::Update(delta);

    // Input is handled by InputManager via KeyboardListener callbacks

    TmxLayer* groundLayer = nullptr;
    TmxMap* map = gameLevel.GetMap();

    // get the ground layer from the map (use an iterator to go through map's layers)
    for (uint8_t i = 0; i < map->layersLength; i++) {
        if ((map->layers[i].type == LAYER_TYPE_TILE_LAYER) &&
            (strcmp(map->layers[i].name, GROUND_LAYER_NAME) == 0)) {
            groundLayer = &map->layers[i];
            break;
        }
    }

    isGrounded =
        CheckCollisionTMXTileLayersRec(map, groundLayer, 1, GetRect(), &tmxCollisionObject);

    // compare player object position with collisionObject position, if player is below the
    // collisionObjet, set player position to stand on colision object (bottom of playet is equal to
    // collisionObject top)
    if (isGrounded && (tmxCollisionObject.y <= (position.y + GetRect().height))) {
        position.y = tmxCollisionObject.y - GetRect().height +
                     1;  // set player position to stand on the collision object
    }

    // Gravity
    if (!isGrounded || wantsToJump) {
        // Apply gravity only if not grounded
        velocity.y += 800.0f * delta;  // gravity constant
        wantsToJump = false;           // reset jump request after applying jump
    } else {
        velocity.y = 0;  // reset vertical velocity when grounded
    }

    // Update vertical position
    position.y += velocity.y * delta;

    // debug output to see current y velocity, y position and grounded state
    // TraceLog(LOG_INFO, "Y Velocity: %f, Y Position: %f, IsGrounded: %s", velocity.y, position.y,
    // isGrounded ? "true" : "false");
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
