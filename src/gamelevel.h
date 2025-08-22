#pragma once

#include <vector>
#include <memory>
#include <functional>
#include "raytmx.h"
#include "actor.h"
#include "Config.hpp"
#include "player.h"

class GameLevel {
   public:
    /*
     * @brief Construct a new GameLevel
     */
    GameLevel(const char* mapFileName);

    /*
     * @brief Update all actors and internal state for the level
     *
     * @param delta Time since last frame in seconds
     */
    void UpdateAll(float delta);

    /*
     * @brief Render the level and all actors
     */
    void Render();

    /*
     * @brief Return a pointer to the single Player actor in this level
     *
     * @return Player* Pointer to the Player instance or nullptr
     */
    Player* GetPlayer() const;

    // Accessor for the TMX map
    TmxMap* GetMap() const { return map; }
    // Accessors for actors in this level
    const std::vector<std::unique_ptr<Actor>>& GetActors() const { return actors; }

    // Add an actor to the level
    template <typename T, typename... Args>
    T& addActor(Args&&... args) {
        // Create a new actor of type T
        auto actor = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& ref = *actor;  // reference to created actor

        // If adding a Player, ensure there's at most one Player in the level.
        if constexpr (std::is_base_of<Player, T>::value) {
            // Find existing Player and replace it
            for (size_t i = 0; i < actors.size(); ++i) {
                if (dynamic_cast<Player*>(actors[i].get()) != nullptr) {
                    // replace existing Player with the new one
                    actors[i] = std::move(actor);
                    return static_cast<T&>(*actors[i]);
                }
            }
        }

        // Default: append to actor list
        actors.push_back(std::move(actor));
        return ref;
    }

   private:
    TmxMap* map = nullptr;  // Pointer to the TMX map
    // container of actors belonging to this level
    std::vector<std::unique_ptr<Actor>> actors;
    // listeners called when an actor is removed
    std::vector<std::function<void(const Actor&)>> removalListeners;
    // camera object for rendering
    Camera2D camera = {0};
};