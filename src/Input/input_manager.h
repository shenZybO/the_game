#pragma once

#include "keyboard_listener.h"
#include <vector>

/**
 * @brief Singleton input manager that polls keyboard and dispatches press/release events.
 *
 * The `InputManager` centralizes keyboard polling and notifies registered
 * [`KeyboardListener`](src/Input/keyboard_listener.h) instances. Listeners are stored
 * as raw pointers (non-owning); listeners must unregister themselves before destruction.
 */
class InputManager {
public:
    /**
     * @brief Get the singleton instance of InputManager.
     *
     * @return InputManager& Reference to the global instance.
     */
    static InputManager& Instance();

    /**
     * @brief Register a keyboard listener.
     *
     * The manager stores the pointer as non-owning. Duplicate registrations are not checked.
     *
     * @param listener Pointer to a [`KeyboardListener`](src/Input/keyboard_listener.h).
     */
    void RegisterListener(KeyboardListener* listener);

    /**
     * @brief Unregister a keyboard listener.
     *
     * Safe to call even if the listener was not registered.
     *
     * @param listener Pointer to the listener to remove.
     */
    void UnregisterListener(KeyboardListener* listener);

    /**
     * @brief Poll keys and dispatch events; call each frame from main loop.
     *
     * This method translates raw raylib key events into `OnKeyPressed` / `OnKeyReleased`
     * calls on each registered listener.
     */
    void Update();

private:
    InputManager() = default;
    std::vector<KeyboardListener*> listeners;
};
