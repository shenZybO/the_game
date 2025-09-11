#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <string_view>
#include <type_traits>
#include "raytmx.h"
#include "actor.h"
#include "config.hpp"
#include "player.h"

/**
 * @brief Represents a loaded game level, including its map and actors.
 *
 * Loads a TMX map and manages actors, rendering and per-frame updates for the level.
 */
class GameLevel {
   public:
    /**
     * @brief Construct a new GameLevel from a TMX map file.
     *
     * @param mapFileName Path to the TMX map file to load.
     */
    GameLevel(std::string_view mapFileName);

    /**
     * @brief Update all actors and internal state for the level.
     */
    void UpdateAll();

    /**
     * @brief Render the level and all actors.
     */
    void Render();

    /**
     * @brief Return a pointer to the Player actor in this level.
     *
     * @return Player* Pointer to the Player instance or nullptr when no player exists.
     */
    Player* GetPlayer() const;

    /**
     * @brief Return the cached ground layer pointer (non-owning).
     */
    TmxLayer const* GetCachedGroundLayer() const { return groundLayer; }

    /**
     * @brief Accessor for the TMX map pointer.
     */
    TmxMap* GetMap() const { return map; }

    /**
     * @brief Accessor for the list of actors in the level.
     */
    const std::vector<std::unique_ptr<Actor>>& GetActors() const { return actors; }

    /**
     * @brief Add an actor of type T to the level.
     *
     * The actor is constructed in-place and stored in the level's actor container. When
     * adding a Player, any existing Player instance will be replaced.
     */
    template <typename T, typename... Args>
    T& addActor(Args&&... args) {
        // Create a new actor of type T
        auto actor = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& ref = *actor;  // reference to created actor

        // If adding a Player, ensure there's at most one Player in the level.
        if constexpr (std::is_base_of_v<Player, T>) {
            // Store the new Player in the dedicated player slot.
            player = std::move(actor);
            return ref;
        }

        // Default: append to actor list
        actors.push_back(std::move(actor));
        return ref;
    }

    /**
     * @brief Reset level state: keep Player but move to start; recreate other actors from TMX.
     */
    void Reset();

    /**
     * @brief Get the bottom edge of the map.
     *
     * @return float The y-coordinate of the bottom edge of the map.
     */
    float GetMapBottom() const;

   private:
    // Helper to find layer by name
    const TmxLayer* FindLayerByName(const char* name) const;
    // Helper to spawn actors from the TMX actors layer
    void SpawnActorsFromMap(bool createPlayer);
    TmxMap* map = nullptr;  // Pointer to the TMX map
    // Cached pointer to the tile layer named "ground" (non-owning)
    const TmxLayer* groundLayer = nullptr;
    // container of actors belonging to this level
    std::vector<std::unique_ptr<Actor>> actors;
    // Dedicated slot for the Player actor (separate from other actors so it can be drawn on top)
    std::unique_ptr<Player> player;
    // listeners called when an actor is removed
    std::vector<std::function<void(const Actor&)>> removalListeners;
    // camera object for rendering
    Camera2D camera = {0};
};