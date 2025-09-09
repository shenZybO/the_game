#include "input_manager.h"
#include <algorithm>
#include "raylib.h"

InputManager& InputManager::Instance() {
    static InputManager inst;
    return inst;
}

void InputManager::RegisterListener(KeyboardListener* listener) {
    listeners.push_back(listener);
}

void InputManager::UnregisterListener(KeyboardListener* listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void InputManager::Update() {
    // For simplicity, check left, right and space keys for press/release and notify listeners
    const int keysToCheck[] = {KEY_LEFT, KEY_RIGHT, KEY_SPACE};
    for (int k : keysToCheck) {
        if (IsKeyPressed(k)) {
            for (auto l : listeners) l->OnKeyPressed(k);
        }
        if (IsKeyReleased(k)) {
            for (auto l : listeners) l->OnKeyReleased(k);
        }
    }
}
