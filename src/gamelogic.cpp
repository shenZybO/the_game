#include "gamelogic.h"

GameLogic& GameLogic::Instance() {
    static GameLogic instance;
    return instance;
}

void GameLogic::RegisterAction(std::unique_ptr<Action> action) {
    actions.push_back(std::move(action));
}

bool GameLogic::DeregisterAction(Action* actionPtr) {
    for (auto it = actions.begin(); it != actions.end(); ++it) {
        if (it->get() == actionPtr) {
            actions.erase(it);
            return true;
        }
    }
    return false;
}

void GameLogic::Update() {
    // get delta time for this frame
    float delta = GetFrameTime();

    // Perform each action; advance time; remove expired
    for (auto it = actions.begin(); it != actions.end();) {
        Action* a = it->get();
        a->Perform(delta);
        a->AdvanceTime(delta);
        if (a->IsExpired()) {
            it = actions.erase(it);
        } else {
            ++it;
        }
    }
}
