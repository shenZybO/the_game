#include "player.h"
#include "raytmx.h"
#include <cstring>
#include <algorithm>
#include "move.h"
#include "gamelogic.h"
#include "gamelevel.h"
#include "jump.h"
#include "types.h"
#include "collision_system.h"

// Destructor is defined inline in header (unregisters input listener)

/**
 * @brief Draw the player using Actor drawing logic.
 */
void Player::Draw() {
    // Apply fade-out when dying
    if (actorState == Actor::STATE_DYING) {
        float alpha = 1.0f - std::min(deathTimer / PlayerConfig::DEATH_FADE_DURATION, 1.0f);
        Color tint = {255, 255, 255, static_cast<unsigned char>(alpha * 255)};
        if (const Animation2D* anim = GetCurrentAnimation()) {
            anim->Draw(GetPosition(), GetFacingDirection() == GameTypes::Direction::Left, tint, 1.0f);
        }
    } else {
        Actor::Draw();
    }
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

    // Handle dying fade and level reset
    if (actorState == Actor::STATE_DYING) {
        deathTimer += delta;
        if (deathTimer >= PlayerConfig::DEATH_FADE_DURATION) {
            SetLives(lives - 1);
            if (lives > 0) {
                gameLevel.Reset();  // Reset level after fade completes
            } else {
                gameLevel.GameOver();  // no more lives, game over
            }
        }
        return;
    }
}

/**
 * @brief Handle key press events for movement and jumping.
 *
 * Registers Move/Jump actions in the global GameLogic when appropriate keys are pressed.
 */
void Player::OnKeyPressed(int key) {
    if (actorState == Actor::STATE_DYING) return;  // ignore input when dying

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

void Player::Destroy() {
    if (actorState == Actor::STATE_DYING) {
        // dying already, ignore further calls
        return;
    }
    alive = false;  // Mark as not alive to prevent further input/updates
    SetState(Actor::STATE_DYING);
    deathTimer = 0.0f;

    if (activeMoveAction) {
        GameLogic::Instance().DeregisterAction(activeMoveAction);
        activeMoveAction = nullptr;
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

void Player::PlayerInit() {
    InputManager::Instance().RegisterListener(this);
    CollisionSystem::Instance().RegisterListener(this);
    /*
     * Configure a fixed physics collider so animation frame size changes do not
     * affect collision and ground detection.
     */
    SetCollider(PlayerConfig::COLLIDER_OFFSET_X, PlayerConfig::COLLIDER_OFFSET_Y, PlayerConfig::COLLIDER_WIDTH,
                PlayerConfig::COLLIDER_HEIGHT);
}

void Player::ResetState() {
    actorState = Actor::STATE_IDLE;
    alive = true;
    deathTimer = 0.0f;
}

void Player::SetLives(int livesNew) {
    lives = std::clamp(livesNew, 0, PlayerConfig::MAX_LIVES);
}

void Player::OnCollision(Actor& self, Actor& other, const Rectangle& overlap) {
    if (&self == &other) return;
    if (actorState == Actor::STATE_DYING) return;
    // Placeholder: react to any collision by initiating death sequence
    Destroy();
}
