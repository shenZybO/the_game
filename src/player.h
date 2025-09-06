#pragma once
#include "raylib.h"
#include "raytmx.h"
#include "movable.h"
#include "action.h"
#include "keyboard_listener.h"
#include "input_manager.h"
#include "canJump.h"

class Player : public Actor, virtual public Movable, public CanJump, public KeyboardListener {
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
     * @param imagePath Path to the player sprite sheet.
     * @param frameCount Frames in the sprite sheet.
     * @param frameSpeed Frame duration for animation.
     */
    Player(GameLevel& level, float x, float y, float jumpStrength,
           float moveSpeed,
           const char* imagePath, int frameCount, float frameSpeed)
        : Actor(level, imagePath, frameCount, frameSpeed, x, y),
          Movable(*this, moveSpeed),
          CanJump(jumpStrength) {
        InputManager::Instance().RegisterListener(this);
    }

        Player(GameLevel& level, float x, float y, float jumpStrength,
           float moveSpeed,
           const char* imagePath, int frameCount, float frameSpeed,
           const char* moveImagePath, int moveFrameCount, float moveFrameSpeed)
        : Actor(level, imagePath, frameCount, frameSpeed, x, y),
          Movable(*this, moveImagePath, moveFrameCount, moveFrameSpeed, moveSpeed),
          CanJump(jumpStrength) {
        InputManager::Instance().RegisterListener(this);
    }

    /**
     * @brief Destroy the Player and unregister keyboard listener.
     */
    ~Player() { 
      InputManager::Instance().UnregisterListener(this); 
    }

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
};
