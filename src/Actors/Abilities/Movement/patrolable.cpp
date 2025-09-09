#include "patrolable.h"
#include "gamelogic.h"
#include "move.h"


void Patrolable::Update(float delta) {

    // If still falling until ground contact, check grounded flag set by Movable::Update
    if (state == PatrolState::FallingToGround) {
        if (IsGrounded()) {
            // choose initial random direction
            std::uniform_int_distribution<int> dist(0, 1);
            patrolDir = (dist(rng) == 0) ? GameTypes::Direction::Left : GameTypes::Direction::Right;
            state = PatrolState::Moving;
        }
        return;
    }

    if (state == PatrolState::Waiting) {
        waitTimer -= delta;
        if (waitTimer <= 0.0f) {
            // reverse direction and resume moving
            patrolDir = (patrolDir == GameTypes::Direction::Left) ? GameTypes::Direction::Right : GameTypes::Direction::Left;
            state = PatrolState::Moving;
        }
        return;
    }

    // Determine position to check: one tile ahead at the actor's feet
    Rectangle rect = self.GetRect();
    float footY = self.GetPosition().y + rect.height + 1.0f; // just below feet
    float aheadX = (patrolDir == GameTypes::Direction::Left) ? (self.GetPosition().x - 1.0f) : (self.GetPosition().x + rect.width + 1.0f);

    // check if tile under the ahead position exists; if not, stop and wait
    if (!HasGroundTileAt(aheadX, footY)) {
        // reached edge: stop and schedule wait
        state = PatrolState::Waiting;
        waitTimer = 0.6f; // pause before turning
        // deregister move action if active
        if (activeMoveAction) {
            GameLogic::Instance().DeregisterAction(activeMoveAction);
            activeMoveAction = nullptr;
        }
        return;
    }

    if (state == PatrolState::Moving) {
            // Register move action for new direction
            if (!activeMoveAction) {
                auto act = std::make_unique<Move>(self, patrolDir);
                Action* raw = act.get();
                GameLogic::Instance().RegisterAction(std::move(act));
                activeMoveAction = raw;
            }
    }
}