#pragma once
#include "raylib.h"
#include "raytmx.h"
#include "movable.h"
#include "action.h"
#include "keyboard_listener.h"
#include "input_manager.h"

class Player : public Movable, public KeyboardListener {
   public:
    Player(GameLevel& level, float x, float y, float moveSpeed, float jumpStrength,
           const char* imagePath, int frameCount, float frameSpeed)
        : Movable(level, imagePath, frameCount, frameSpeed, x, y, moveSpeed),
          jumpStrength(jumpStrength) {
        InputManager::Instance().RegisterListener(this);
    }
    ~Player() { InputManager::Instance().UnregisterListener(this); }

    void Update(float delta);
    // Override draw to flip sprite when moving left
    virtual void Draw() override;

    // KeyboardListener callbacks
    void OnKeyPressed(int key) override;
    void OnKeyReleased(int key) override;

   private:
    float jumpStrength;
    // Pointer to currently registered Move action (if any)
    Action* activeMoveAction = nullptr;
};
