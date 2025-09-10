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
 * @brief Update movable physics each frame: ground checks, gravity and animation selection.
 */
void Movable::Update(float delta) {
    // Update grounded state via foot sensor hysteresis after physics integration
    UpdateGroundedState(delta);

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

    if (tileX < 0 || tileX >= (int)map->width || tileY < 0 || tileY >= (int)map->height) {
        return false;
    }

    // compute index into tile array
    uint32_t idx = tileY * ground->exact.tileLayer.width + tileX;
    if (idx >= ground->exact.tileLayer.tilesLength) {
        return false;
    }
    
    return ground->exact.tileLayer.tiles[idx] != 0;

}

/**
 * @brief Update grounded flag using a narrow foot sensor and grace period.
 */
void Movable::UpdateGroundedState(float delta) {
    bool groundedNow = false;
    TmxMap* map = self.GetGameLevel().GetMap();
    const TmxLayer* ground = self.GetGameLevel().GetCachedGroundLayer();

    if (map != nullptr && ground != nullptr) {
        Rectangle body = self.GetRect();

        // calculate sensor dimensions and create rectangle
        float sensorWidth = body.width * MoveConfig::FOOT_SENSOR_WIDTH_RATIO;
        
        // Clamp sensor width to reasonable range
        sensorWidth = std::max(body.width * MoveConfig::FOOT_SENSOR_MIN_WIDTH_RATIO, std::min(sensorWidth, body.width));

        Rectangle sensor { body.x + ((body.width - sensorWidth) * 0.5f),
                           body.y + body.height + MoveConfig::FOOT_SENSOR_GAP,
                           sensorWidth,
                           MoveConfig::FOOT_SENSOR_HEIGHT };

        uint32_t hitCount = 0;
        TmxObject* collisionObjects =
            CheckCollisionTMXTileLayersRecAllAlloc(map, ground, 1, sensor, &hitCount);

        // in case of no hits nothing to do, make sure to free memory
        if (hitCount == 0 || collisionObjects == NULL) {
            if (collisionObjects != NULL) {
                MemFree(collisionObjects);
            }
        } else {
            groundedNow = true;
            // go through all hits and find the highest top
            float highestCollisionTop = collisionObjects[0].y;  // take the first hit to start with
            for (uint32_t i = 1; i < hitCount; i++) {
                if (collisionObjects[i].y < highestCollisionTop) {
                    highestCollisionTop = collisionObjects[i].y;
                }
            }

            // we don't need the collision objects anymore
            MemFree(collisionObjects);

            // If grounded and moving downward (or resting), snap actor to stand on the highest ground tile
            if (velocity.y >= 0.0f) {
                self.GetPosition().y = highestCollisionTop - body.height + 1;
                velocity.y = 0.0f;
            }
        }
    }

    // delay setting of isGrounded = false to avoid flicker when walking off edges
    if (groundedNow) {
        isGrounded = true;
        timeSinceLastGround = 0.0f;
    } else {
        timeSinceLastGround += delta;
        if (timeSinceLastGround > MoveConfig::GROUND_GRACE_TIME) {
            isGrounded = false;
        }
    }
}