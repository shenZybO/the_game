#include "movable.h"
#include "gamelevel.h"
#include "raytmx.h"

/**
 * @brief Move the owning actor by the specified delta and clamp to map bounds.
 *
 * @param dx Horizontal world delta.
 * @param dy Vertical world delta.
 */
void Movable::MoveBy(float dx, float dy) {
    TmxMap* map = self.GetGameLevel().GetMap();

    Vector2& position = self.GetPosition();
    Rectangle rect = self.GetRect();
    position.x += dx;
    position.y += dy;

    // clip the position to ensure it stays within the bounds of the map
    if (position.x < 0) {
        position.x = 0;
    }
    if (position.x > map->width * map->tileWidth - rect.width) {
        position.x = map->width * map->tileWidth - rect.width;
    }
    if (position.y > map->height * map->tileHeight - rect.height) {
        position.y = map->height * map->tileHeight - rect.height;
        velocity.y = 0;  // reset vertical velocity when clamped to bottom
    }
}

/**
 * @brief Perform swept ground collision and snap actor to the ground when appropriate.
 *
 * Uses the cached ground layer (if available) and performs a swept test downwards
 * to determine the nearest collision top. Snaps the actor on top and clears vertical
 * velocity when a ground collision within tolerance is found.
 */
bool Movable::CheckAndFixGroundCollision(float delta) {
    const GameLevel& gameLevel = self.GetGameLevel();

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
        Rectangle swept = self.GetRect();
        swept.height += verticalDisplacement;  // extend downwards to include the travel path

        TmxObject* collisionObjects =
            CheckCollisionTMXTileLayersRecAllAlloc(map, ground, 1, swept, &hitCount);

        if (hitCount == 0 || collisionObjects == NULL) {
            if (collisionObjects != NULL) MemFree(collisionObjects);
            return false;
        }

        // Only snap if collision top lies between previous bottom and new bottom (within tolerance)
        float bottomBefore = self.GetPosition().y + self.GetRect().height;
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
        self.GetPosition().y = bestCollisionTop - self.GetRect().height + 1;
        velocity.y = 0.0f;
        return true;
    }

    return false;
}

/**
 * @brief Update movable physics each frame: ground checks, gravity and animation selection.
 */
void Movable::Update(float delta) {

    isGrounded = CheckAndFixGroundCollision(delta);

    // apply general gravity if not grounded (falling possible even for Actor not able to jump)
    if (!isGrounded || self.GetState() == Actor::STATE_JUMPING) {
        velocity.y += MoveConfig::GRAVITY_CONSTANT * delta;  // gravity constant
        // Update vertical position
        self.GetPosition().y += velocity.y * delta; 
    } else {
        velocity.y = 0;  // reset vertical velocity when grounded
    }

    // resolve facing direction
    if(velocity.x < 0) {
        self.SetFacingDirection(GameTypes::Direction::Left);
    } else if(velocity.x > 0) {
        self.SetFacingDirection(GameTypes::Direction::Right);
    } // else keep current facing

    if (velocity.y < 0) {
        self.SetState(Actor::STATE_JUMPING);
    } else if (!isGrounded && velocity.y > 0) {
        self.SetState(Actor::STATE_FALLING);
    } else if (velocity.x < 0) {
        self.SetState(Actor::STATE_MOVING_LEFT);
    } else if (velocity.x > 0) {
        self.SetState(Actor::STATE_MOVING_RIGHT);
    } else {
        self.SetState(Actor::STATE_IDLE);
        self.ResetToDefaultAnimation();
    }

    if (fallingAnimation && self.GetState() == Actor::STATE_FALLING) {
        self.SetCurrentAnimation(fallingAnimation);
    } else if (movingAnimation && (self.GetState() == Actor::STATE_MOVING_LEFT || self.GetState() == Actor::STATE_MOVING_RIGHT)) {
        self.SetCurrentAnimation(movingAnimation);
    }

}

/**
 * @brief Query whether the ground layer contains a tile at given world coordinates.
 *
 * Returns false when map or ground layer is not available.
 */
bool Movable::HasGroundTileAt(float worldX, float worldY) const {
    TmxMap* map = self.GetGameLevel().GetMap();
    TmxLayer const* ground = self.GetGameLevel().GetCachedGroundLayer();
    if (map == nullptr || ground == nullptr) return false;

    // Convert world coords to tile coords
    int tileX = static_cast<int>(worldX) / map->tileWidth;
    int tileY = static_cast<int>(worldY) / map->tileHeight;

    if (tileX < 0 || tileX >= (int)map->width || tileY < 0 || tileY >= (int)map->height) return false;

    // compute index into tile array
    uint32_t idx = tileY * ground->exact.tileLayer.width + tileX;
    if (idx >= ground->exact.tileLayer.tilesLength) return false;
    
    return ground->exact.tileLayer.tiles[idx] != 0;
}