#pragma once

/* Simple keyboard listener interface for receiving key press/release events. */
class KeyboardListener {
public:
    virtual ~KeyboardListener() = default;
    virtual void OnKeyPressed(int key) = 0;
    virtual void OnKeyReleased(int key) = 0;
};
