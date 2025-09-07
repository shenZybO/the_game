#pragma once
#include "raylib.h"
#include "raytmx.h"
#include "movable.h"
#include "action.h"
#include "keyboard_listener.h"
#include "input_manager.h"
#include "jumpable.h"

class Player : public Actor, virtual public Movable, public Jumpable, public KeyboardListener {
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
    Player(GameLevel& level, float x, float y, float jumpStrength,
           float moveSpeed, GameTypes::AnimationData idleAnim)
        : Actor(level, idleAnim, x, y),
          Movable(*this, moveSpeed),
          Jumpable(jumpStrength) {
            PlayerInit();
    }

    Player(GameLevel& level, float x, float y, float jumpStrength,
           float moveSpeed, GameTypes::AnimationData idleAnim, GameTypes::AnimationData moveAnim)
        : Actor(level, idleAnim, x, y),
          Movable(*this, moveAnim),
          Jumpable(jumpStrength) {
            PlayerInit();
    }

    Player(GameLevel& level, float x, float y, float jumpStrength,
           float moveSpeed, GameTypes::AnimationData idleAnim, GameTypes::AnimationData moveAnim,
           GameTypes::AnimationData jumpAnim)
        : Actor(level, idleAnim, x, y),
          Movable(*this, moveAnim, moveSpeed),
          Jumpable(jumpStrength, jumpAnim) {
            PlayerInit();
    }

    /**
     * @brief Destroy the Player and unregister keyboard listener.
     */
    ~Player() { 
      InputManager::Instance().UnregisterListener(this); 
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

  private:
    void PlayerInit() {
        InputManager::Instance().RegisterListener(this);
    }
};
