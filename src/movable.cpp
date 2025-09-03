#include "movable.h"
#include "gamelevel.h"
#include "raytmx.h"

void Movable::MoveBy(float dx, float dy) {
    TmxMap* map = gameLevel.GetMap();

    position.x += dx;
    position.y += dy;

    // clip the position to ensure it stays within the bounds of the map
    if (position.x < 0) {
        position.x = 0;
    }
    if (position.x > map->width * map->tileWidth - GetRect().width) {
        position.x = map->width * map->tileWidth - GetRect().width;
    }
    if (position.y > map->height * map->tileHeight - GetRect().height) {
        position.y = map->height * map->tileHeight - GetRect().height;
        velocity.y = 0;  // reset vertical velocity when clamped to bottom
    }
}

// Helper: check collision with given ground layer and, if colliding, snap the actor to stand on it
bool Movable::CheckAndFixGroundCollision(float delta) {
    TmxLayer const* ground = gameLevel.GetCachedGroundLayer();
    TmxMap* map = gameLevel.GetMap();

    // No layer or map to check against
    if (ground == nullptr || map == nullptr) {
        return false;
    }

    // Compute vertical displacement for this frame
    float verticalDisplacement = velocity.y * delta;

    // Swept collision only makes sense when moving downwards or standing (if standing, we still
    // want to snap to ground, otherwise we fall through)
    if (verticalDisplacement >= 0.0f) {
        uint32_t hitCount = 0;
        // Build a swept rectangle that covers from current rect down to the expected rect
        Rectangle swept = GetRect();
        swept.height += verticalDisplacement;  // extend downwards to include the travel path

        TmxObject* collisionObjects =
            CheckCollisionTMXTileLayersRecAllAlloc(map, ground, 1, swept, &hitCount);

        if (hitCount == 0 || collisionObjects == NULL) {
            if (collisionObjects != NULL) MemFree(collisionObjects);
            return false;
        }

        // Only snap if collision top lies between previous bottom and new bottom (within tolerance)
        float bottomBefore = position.y + GetRect().height;
        float bottomAfter = bottomBefore + verticalDisplacement;
        float bestCollisionTop = collisionObjects[0].y;  // take the first hit for now;
        bool snap = false;

        for (uint32_t i = 0; i < hitCount; i++) {
            float collisionTop = collisionObjects[i].y;
            if (collisionTop >= (bottomBefore - MoveConfig::VERTICAL_SNAP_TOLERANCE) &&
                collisionTop <= (bottomAfter + MoveConfig::VERTICAL_SNAP_TOLERANCE)) {
                snap = true;
                if (collisionTop > bestCollisionTop) bestCollisionTop = collisionTop;
            }
        }

        MemFree(collisionObjects);

        if (!snap) return false;
        // snap to top of collision and stop vertical motion
        position.y = bestCollisionTop - GetRect().height + 1;
        velocity.y = 0.0f;
        return true;
    }

    return false;
}

// Override of Actor::Update: keep animation update, then update grounded state and apply gravity
void Movable::Update(float delta) {
    // call base implementation (updates animation)
    // Update both default and moving animations as needed. The moving animation is updated when the
    // actor is in a moving state so it is ready for drawing.
    Actor::Update(delta);
    if (HasMovingAnimation()) {
        if (actorState == Actor::STATE_MOVING_LEFT || actorState == Actor::STATE_MOVING_RIGHT) {
            movingAnimation->Update(delta);
        } else {
            // Keep the moving animation at its first frame when not moving so it doesn't advance.
        }
    }

    isGrounded = CheckAndFixGroundCollision(delta);
}

// Override Draw to choose moving animation when appropriate
void Movable::Draw() {
    // If moving state and we have a moving animation, draw it. Otherwise fall back to default animation.
    if ((actorState == Actor::STATE_MOVING_LEFT || actorState == Actor::STATE_MOVING_RIGHT) && HasMovingAnimation()) {
        bool flipped = (actorState == Actor::STATE_MOVING_LEFT);
        movingAnimation->Draw(position, WHITE, 1.0f, flipped);
    } else {
        // Use Actor's draw behavior (which draws defaultAnimation)
        Actor::Draw();
    }
}