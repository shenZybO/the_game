#include "movable.h"
#include "gamelevel.h"
#include "raytmx.h"
#include <algorithm>

/**
 * @brief Move the owning actor by the specified delta and clamp to map bounds.
 *
 * @param dx Horizontal world delta.
 * @param dy Vertical world delta.
 */
void Movable::MoveBy(float dx, float dy) {
    TmxMap* map = self.GetGameLevel().GetMap();

    Vector2 position = self.GetPosition();
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
    self.SetPosition(position.x, position.y);
}

/**
 * @brief Update movable physics each frame: ground checks, gravity and animation selection.
 */
void Movable::Update(float delta) {
    if (!self.IsAlive()) {
        // set velocity to 0
        velocity.x = 0;
        velocity.y = 0;
        return;
    }
    // Update grounded state via foot sensor hysteresis after physics integration
    UpdateGroundedState(delta);

    // apply gravity if not grounded or if currently in jumping movement state
    if (!isGrounded || movementState == MovementState::Jumping) {
        velocity.y += MoveConfig::GRAVITY_CONSTANT * delta;  // gravity constant
        // Update vertical position
        self.SetPosition(self.GetPosition().x, self.GetPosition().y + (velocity.y * delta));
    } else {
        velocity.y = 0;  // reset vertical velocity when grounded
    }

    // resolve facing direction
    if (velocity.x < 0) {
        self.SetFacingDirection(GameTypes::Direction::Left);
    } else if (velocity.x > 0) {
        self.SetFacingDirection(GameTypes::Direction::Right);
    }  // else keep current facing

    if (velocity.y < 0) {
        movementState = MovementState::Jumping;
    } else if (!isGrounded && velocity.y > 0) {
        movementState = MovementState::Falling;
    } else if (velocity.x < 0) {
        movementState = MovementState::MovingLeft;
    } else if (velocity.x > 0) {
        movementState = MovementState::MovingRight;
    } else {
        movementState = MovementState::Idle;
        self.ResetToDefaultAnimation();
    }

    if (prevMovementState != MovementState::Falling && fallingAnimation && movementState == MovementState::Falling) {
        self.SetCurrentAnimation(fallingAnimation);
    } else if ((prevMovementState != MovementState::MovingLeft && prevMovementState != MovementState::MovingRight) &&
               movingAnimation &&
               (movementState == MovementState::MovingLeft || movementState == MovementState::MovingRight)) {
        self.SetCurrentAnimation(movingAnimation);
    }

    // Death by falling out of the map bottom
    if (self.GetPosition().y > self.GetGameLevel().GetMapBottom() - MoveConfig::DEATH_FALL_MARGIN) {
        self.Destroy();
    }

    prevMovementState = movementState;
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
        sensorWidth = std::clamp(sensorWidth, body.width * MoveConfig::FOOT_SENSOR_MIN_WIDTH_RATIO, body.width);

        Rectangle sensor{body.x + ((body.width - sensorWidth) * 0.5f),
                         body.y + body.height + MoveConfig::FOOT_SENSOR_GAP, sensorWidth,
                         MoveConfig::FOOT_SENSOR_HEIGHT};

        uint32_t hitCount = 0;
        TmxObject* collisionObjects = CheckCollisionTMXTileLayersRecAllAlloc(map, ground, 1, sensor, &hitCount);

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

            // If grounded and moving downward (or resting), snap actor to stand on the highest
            // ground tile
            if (velocity.y >= 0.0f) {
                self.SetPosition(self.GetPosition().x, highestCollisionTop - body.height + 1);
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