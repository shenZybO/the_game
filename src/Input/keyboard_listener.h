#pragma once

/**
 * @brief Interface for objects that want to receive keyboard events.
 *
 * Implement this interface to receive keyboard notifications from
 * [`InputManager`](src/Input/input_manager.h). Implementations must register
 * themselves with the `InputManager` and ensure they are unregistered before
 * destruction to avoid dangling pointers.
 */
class KeyboardListener {
   public:
    virtual ~KeyboardListener() = default;

    /**
     * @brief Keyboard key pressed callback.
     *
     * @param key Raylib key code (e.g. KEY_LEFT, KEY_RIGHT, KEY_SPACE).
     */
    virtual void OnKeyPressed(int key) = 0;

    /**
     * @brief Keyboard key released callback.
     *
     * @param key Raylib key code (e.g. KEY_LEFT, KEY_RIGHT, KEY_SPACE).
     */
    virtual void OnKeyReleased(int key) = 0;
};
