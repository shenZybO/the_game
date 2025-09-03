#pragma once
#include "raylib.h"
#include "raytmx.h"
#include "movable.h"
#include "action.h"
#include "keyboard_listener.h"
#include "input_manager.h"

class Player : public Movable, public KeyboardListener {
   public:
    /**
     * @brief Construct a new Player instance.
     *
     * Registers the player as a keyboard listener on construction.
     *
     * @param level Reference to the owning GameLevel.
     * @param x Initial X position.
     * @param y Initial Y position.
     * @param moveSpeed Horizontal move speed.
     * @param jumpStrength Strength of jump impulse.
     * @param imagePath Path to the player sprite sheet.
     * @param frameCount Frames in the sprite sheet.
     * @param frameSpeed Frame duration for animation.
     */
    Player(GameLevel& level, float x, float y, float moveSpeed, float jumpStrength,
           const char* imagePath, int frameCount, float frameSpeed)
        : Movable(level, imagePath, frameCount, frameSpeed, x, y, moveSpeed),
          jumpStrength(jumpStrength) {
        InputManager::Instance().RegisterListener(this);
    }

    /**
     * @brief Destroy the Player and unregister keyboard listener.
     */
    ~Player() { InputManager::Instance().UnregisterListener(this); }

    /**
     * @brief Update player logic each frame.
     *
     * @param delta Time elapsed in seconds since last frame.
     */
    void Update(float delta);

    /**
     * @brief Draw the player; overrides Movable/Actor drawing behavior to handle flipping.
     */
    virtual void Draw() override;

    /**
     * @brief KeyboardListener callback: key pressed event.
     */
    void OnKeyPressed(int key) override;

    /**
     * @brief KeyboardListener callback: key released event.
     */
    void OnKeyReleased(int key) override;

   private:
    float jumpStrength; /**< Jump impulse strength. */
    Action* activeMoveAction = nullptr; /**< Currently active Move action, if any. */
};
