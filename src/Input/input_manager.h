#pragma once

#include "keyboard_listener.h"
#include <vector>

/* Singleton input manager that polls keyboard and dispatches press/release events to listeners. */
class InputManager {
   public:
    static InputManager& Instance();

    void RegisterListener(KeyboardListener* listener);
    void UnregisterListener(KeyboardListener* listener);

    // Poll keys and dispatch events; call each frame
    void Update();

   private:
    InputManager() = default;
    std::vector<KeyboardListener*> listeners;
};
