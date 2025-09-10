#include "gamelevel.h"
#include <algorithm>
#include "config.hpp"
#include "asset_manager.h"

/**
 * @brief Construct and initialize a GameLevel from a TMX map file.
 *
 * Loads the map and caches commonly used layers such as the ground layer.
 */
GameLevel::GameLevel(std::string_view mapFileName) {
    // Load the TMX map from the specified file
    map = LoadTMX(AssetManager::GetAssetPath(mapFileName).string().c_str());
    if (map == nullptr) {
        TraceLog(LOG_ERROR, "Failed to load TMX map: %s", mapFileName);
    }

    // Cache the ground layer pointer to avoid repeated name lookups
    groundLayer = nullptr;
    if (map != nullptr) {
        for (uint32_t i = 0; i < map->layersLength; ++i) {
            if (map->layers[i].type == LAYER_TYPE_TILE_LAYER) {
                if (strcmp(map->layers[i].name, "ground") == 0) {
                    groundLayer = const_cast<TmxLayer*>(&map->layers[i]);
                    break;
                }
            }
        }
    }

    // initialize the camera
    camera.zoom = 2.0f;
    camera.offset = {(float)Config::SCREEN_WIDTH / 2, (float)Config::SCREEN_HEIGHT / 2};
    camera.rotation = 0.0f;
}

/**
 * @brief Update all actors in the level and perform cleanup of dead actors.
 */
void GameLevel::UpdateAll() {
    // Get delta time for this frame
    float delta = GetFrameTime();

    // Update all non-player actors in the level
    for (auto& actor : actors) {
        if (actor->IsAlive()) {
            actor->Update(delta);
        }
    }

    // Update player separately if present
    if (player && player->IsAlive()) {
        player->Update(delta);
    }

    // Cleanup dead actors and noify removal listeners
    auto iterator =
        std::remove_if(actors.begin(), actors.end(), [&](const std::unique_ptr<Actor>& actor) {
            // Check if the actor is not alive (destroyed)
            if (!actor->IsAlive()) {
                // Notify all removal listeners
                for (const auto& listener : removalListeners) {
                    listener(*actor);
                }
                return true;  // Remove this actor
            }
            return false;  // Keep this actor
        });

    // Erase the dead actors from the vector
    if (iterator != actors.end()) {
        actors.erase(iterator, actors.end());
    }

    // If the dedicated player became dead, notify listeners and clear the slot
    if (player && !player->IsAlive()) {
        for (const auto& listener : removalListeners) {
            listener(*player);
        }
        player.reset();
    }
}

/**
 * @brief Render the TMX map and all actors using the level camera.
 */
void GameLevel::Render() {
    // Camera follows player, but clamp to map edges
    Vector2 camTarget = GetPlayer() ? GetPlayer()->GetPosition() : Vector2{0, 0};
    float halfScreenW = Config::SCREEN_WIDTH / (2 * camera.zoom);
    float halfScreenH = Config::SCREEN_HEIGHT / (2 * camera.zoom);
    float maxX = map->width * map->tileWidth - halfScreenW;
    float maxY = map->height * map->tileHeight - halfScreenH;
    if (camTarget.x < halfScreenW) camTarget.x = halfScreenW;
    if (camTarget.x > maxX) camTarget.x = maxX;
    if (camTarget.y < halfScreenH) camTarget.y = halfScreenH;
    if (camTarget.y > maxY) camTarget.y = maxY;
    camera.target = camTarget;

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    AnimateTMX(map);
    DrawTMX(map, &camera, 0, 0, WHITE);

    // Render all non-player actors first so the player is drawn on top
    for (const auto& actor : actors) {
        if (actor->IsAlive()) {
            actor->Draw();
        }
    }

    // Render player last so it appears on top of other actors
    if (player && player->IsAlive()) {
        player->Draw();
    }

    EndMode2D();
    DrawFPS(10, 10);
    EndDrawing();
}

Player* GameLevel::GetPlayer() const {
    // Return the dedicated player instance if present
    return player.get();
}

/**
 * @brief Find and return the tile layer named "ground".
 */
TmxLayer* GameLevel::GetGroundLayer() const {
    TmxLayer* result = nullptr;
    if (map == nullptr) {
        return result;
    }

    for (uint32_t i = 0; i < map->layersLength; ++i) {
        if (map->layers[i].type == LAYER_TYPE_TILE_LAYER) {
            if (strcmp(map->layers[i].name, "ground") == 0) {
                result = const_cast<TmxLayer*>(&map->layers[i]);
                break;
            }
        }
    }

    return result;
}