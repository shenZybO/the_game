#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <string_view>
#include <type_traits>
#include "raytmx.h"
#include "actor.h"
#include "Config.hpp"
#include "player.h"

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
     * @brief Find and return the TMX layer named "ground" from the map.
     *
     * @return TmxLayer* pointer to the ground layer or nullptr when not found or map missing.
     */
    TmxLayer* GetGroundLayer() const;

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

   private:
    TmxMap* map = nullptr;  // Pointer to the TMX map
    // Cached pointer to the tile layer named "ground" (non-owning)
    TmxLayer* groundLayer = nullptr;
    // container of actors belonging to this level
    std::vector<std::unique_ptr<Actor>> actors;
    // Dedicated slot for the Player actor (separate from other actors so it can be drawn on top)
    std::unique_ptr<Player> player;
    // listeners called when an actor is removed
    std::vector<std::function<void(const Actor&)>> removalListeners;
    // camera object for rendering
    Camera2D camera = {0};
};