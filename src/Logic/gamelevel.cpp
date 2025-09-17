#include <algorithm>
#include "gamelevel.h"
#include "gamelogic.h"
#include "config.hpp"
#include "asset_manager.h"
#include "enemy.h"
#include "texture_manager.h"
#include "collision_system.h"

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
    groundLayer = FindLayerByName(GameConfig::GROUND_LAYER_NAME.data());

    // Spawn actors defined in TMX
    SpawnActorsFromMap(true);

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

    // Update player separately if present, keep updating even if dead for respawn logic
    if (player) {
        player->Update(delta);
    }

    // Cleanup dead actors and noify removal listeners
    auto iterator = std::remove_if(actors.begin(), actors.end(), [&](const std::unique_ptr<Actor>& actor) {
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
    // do not remove player - keep for respawn

    // Run collision detection after all movement/animation updates
    CollisionSystem::Instance().Update(actors, player.get());
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

    // Render player last so it appears on top of other actors, keep drawing even if dead for death
    // animation
    if (player) {
        player->Draw();
    }

    EndMode2D();
    // HUD (lives, etc.) drawn after world but before FPS
    DrawHUD();
    DrawFPS(10, 10);
    EndDrawing();
}

Player* GameLevel::GetPlayer() const {
    // Return the dedicated player instance if present
    return player.get();
}

float GameLevel::GetMapBottom() const {
    if (map == nullptr) return 0.0f;
    return map->height * map->tileHeight;
}

/**
 * @brief Utility: find a layer by name.
 */
const TmxLayer* GameLevel::FindLayerByName(const char* name) const {
    if (map == nullptr || name == nullptr) return nullptr;
    for (uint32_t i = 0; i < map->layersLength; ++i) {
        if (map->layers[i].name && strcmp(map->layers[i].name, name) == 0) {
            return &map->layers[i];
        }
    }
    return nullptr;
}

/**
 * @brief Spawn actors from the TMX "actors" layer.
 */
void GameLevel::SpawnActorsFromMap(bool createPlayer) {
    const TmxLayer* actorsLayer = FindLayerByName(GameConfig::ACTORS_LAYER_NAME.data());
    if (actorsLayer == nullptr || actorsLayer->type != LAYER_TYPE_OBJECT_GROUP) {
        return;
    }

    const TmxObjectGroup& group = actorsLayer->exact.objectGroup;
    for (uint32_t i = 0; i < group.objectsLength; ++i) {
        const TmxObject& obj = group.objects[i];
        if (obj.visible && obj.name != nullptr) {
            // Position from Tiled is top-left - that is what the actors expect
            if (strcmp(obj.name, GameConfig::PLAYER_OBJECT_NAME.data()) == 0) {
                // Create player if not existing yet
                if (createPlayer || !player) {
                    addActor<Player>(obj.x, obj.y, PlayerConfig::DEFAULT_JUMP_STRENGTH,
                                     PlayerConfig::DEFAULT_MOVE_SPEED, PlayerConfig::IDLE_ANIM, PlayerConfig::WALK_ANIM,
                                     PlayerConfig::JUMP_ANIM, PlayerConfig::FALL_ANIM);
                } else {
                    // Move existing player to start position
                    player->SetPosition(obj.x, obj.y);
                }
            } else if (strcmp(obj.name, GameConfig::ZOMBIE_OBJECT_NAME.data()) == 0) {
                addActor<Enemy>(obj.x, obj.y, EnemyConfig::DEFAULT_MOVE_SPEED, EnemyConfig::IDLE_ANIM,
                                EnemyConfig::WALK_ANIM);
            }
        }
    }
}

/**
 * @brief Reset level: keep Player instance, move to start, respawn other actors from TMX.
 */
void GameLevel::Reset() {
    // Clear all actions from GameLogic
    GameLogic::Instance().Cleanup();
    // Remove non-player actors
    actors.clear();
    // Recreate non-player actors from map and move player to start position
    SpawnActorsFromMap(false);
    // reset player state
    if (player) {
        player->ResetState();
    }
}

void GameLevel::GameOver() {
    levelState = LevelState::LEVEL_NO_LIVES;
}

void GameLevel::DrawHUD() {
    if (player) {
        // Draw lives as heart icons in upper-right corner
        Texture2D& fullTex = TextureManager::Instance().GetTexture(PlayerConfig::HEART_FULL_TEXTURE);
        Texture2D& emptyTex = TextureManager::Instance().GetTexture(PlayerConfig::HEART_EMPTY_TEXTURE);
        int tileW = map ? (int)map->tileWidth : fullTex.width;

        for (int i = 0; i < PlayerConfig::MAX_LIVES; ++i) {
            int x = Config::SCREEN_WIDTH - ((PlayerConfig::MAX_LIVES - i) * tileW);
            Texture2D& lifeTexture = (i < player->GetLives()) ? fullTex : emptyTex;
            DrawTexture(lifeTexture, x, 0, WHITE);
        }
    }

    // TODO: game over message only temporarily here - move to class handling
    // level/game state one implementation exists
    if (levelState == LevelState::LEVEL_NO_LIVES) {
        int fontSize = 40;
        int w = MeasureText(GameConfig::GAME_OVER_TEXT.data(), fontSize);
        int x = (Config::SCREEN_WIDTH - w) / 2;
        int y = Config::SCREEN_HEIGHT / 3;

        // draw shadow and main text
        DrawText(GameConfig::GAME_OVER_TEXT.data(), x + 2, y + 2, fontSize, BLACK);
        DrawText(GameConfig::GAME_OVER_TEXT.data(), x, y, fontSize, RED);

        // After short delay exit
        static float goTimer = 0.0f;
        goTimer += GetFrameTime();
        if (goTimer > 1.5f) {
            levelState = LevelState::LEVEL_GAME_OVER;
        }
    }
}