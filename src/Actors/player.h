#pragma once
#include "raylib.h"
#include "raytmx.h"
#include "movable.h"
#include "action.h"
#include "keyboard_listener.h"
#include "input_manager.h"
#include "jumpable.h"
#include "config.hpp"
#include "collision_listener.h"
#include "collision_system.h"

/**
 * @brief Player actor representing the user-controlled character.
 *
 * The Player composes movement, jumping and input-listening behaviors. It registers
 * itself with the `InputManager` to receive keyboard events and updates animation
 * and physics state each frame.
 */
class Player : public Actor,
               virtual public Movable,
               public Jumpable,
               public KeyboardListener,
               public ICollisionListener {
public:
    /**
     * @brief Construct a new Player instance.
     *
     * Registers the player as a keyboard listener on construction.
     *
     * @param level Reference to the owning GameLevel.
     * @param x Initial X position.
     * @param y Initial Y position.
     * @param jumpStrength Strength of jump impulse.
     * @param moveSpeed Horizontal move speed.
     * @param imagePath Path to the player sprite sheet.
     * @param frameCount Frames in the sprite sheet.
     * @param frameSpeed Frame duration for animation.
     */
    Player(GameLevel& level, float x, float y, float jumpStrength, float moveSpeed, GameTypes::AnimationData idleAnim)
        : Actor(level, idleAnim, x, y), Movable(*this, moveSpeed), Jumpable(jumpStrength) {
        PlayerInit();
    }

    Player(GameLevel& level, float x, float y, float jumpStrength, float moveSpeed, GameTypes::AnimationData idleAnim,
           GameTypes::AnimationData moveAnim)
        : Actor(level, idleAnim, x, y), Movable(*this, moveAnim, moveSpeed), Jumpable(jumpStrength) {
        PlayerInit();
    }

    Player(GameLevel& level, float x, float y, float jumpStrength, float moveSpeed, GameTypes::AnimationData idleAnim,
           GameTypes::AnimationData moveAnim, GameTypes::AnimationData jumpAnim)
        : Actor(level, idleAnim, x, y), Movable(*this, moveAnim, moveSpeed), Jumpable(jumpStrength, jumpAnim) {
        PlayerInit();
    }

    Player(GameLevel& level, float x, float y, float jumpStrength, float moveSpeed, GameTypes::AnimationData idleAnim,
           GameTypes::AnimationData moveAnim, GameTypes::AnimationData jumpAnim, GameTypes::AnimationData fallAnim)
        : Actor(level, idleAnim, x, y),
          Movable(*this, moveAnim, fallAnim, moveSpeed),
          Jumpable(jumpStrength, jumpAnim) {
        PlayerInit();
    }

    /**
     * @brief Destroy the Player and unregister keyboard listener.
     */
    ~Player() {
        InputManager::Instance().UnregisterListener(this);
        CollisionSystem::Instance().UnregisterListener(this);
    }
    // Non-copyable/movable: listener registration ties lifetime to this instance
    Player(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) = delete;

    /**
     * @brief Update player logic each frame.
     *
     * @param delta Time elapsed in seconds since last frame.
     */
    void Update(float delta) override;

    /**
     * @brief Draw the player; overrides Movable/Actor drawing behavior to handle flipping.
     */
    void Draw() override;

    /**
     * @brief KeyboardListener callback: key pressed event.
     */
    void OnKeyPressed(int key) override;

    /**
     * @brief KeyboardListener callback: key released event.
     */
    void OnKeyReleased(int key) override;

    /**
     * @brief Override to start a dying sequence (fade out) and reset the level.
     */
    void Destroy() override;

    /**
     * @brief Take damage and trigger appropriate reactions.
     */
    void TakeDamage();

    /**
     * @brief Reset player state to initial conditions (alive, idle, no death timer).
     */
    void ResetState();

    /**
     * @brief Collision callback from CollisionSystem.
     */
    void OnCollision(Actor& self, Actor& other, const Rectangle& overlap) override;

    Actor& GetCollisionActor() override { return *this; }

    /**
     * @brief Get the current number of remaining lives.
     */
    int GetLives() const noexcept { return lives; }

    /**
     * @brief Set the current number of lives (clamped to [0, maxLives]).
     */
    void SetLives(int livesNew);

    /**
     * @brief Increase the current number of lives by one (up to maxLives).
     */
    void AddLife() { SetLives(lives + 1); }

    /**
     * @brief Override to add specific effects when changing animation (e.g. blinking).
     */
    void SetCurrentAnimation(std::shared_ptr<IAnimation2D> anim) override;

private:
    // timer for timed actor states (state is left after timer runs out)
    float stateTimer = 0.0f;
    // remaining lives
    int lives = PlayerConfig::START_LIVES;

    // Helper to initialize player-specific settings
    void PlayerInit();
    // Re-set the current animation to apply any effects (e.g. blinking).
    void RefreshAnimation();
};
