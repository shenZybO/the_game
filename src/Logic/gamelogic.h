#pragma once

#include "action.h"
#include <vector>
#include <memory>

class GameLogic {
   public:
    static GameLogic& Instance();

    // register an action - ownership is transferred to GameLogic
    void RegisterAction(std::unique_ptr<Action> action);

    // deregister and destroy an action by pointer (returns true if found)
    bool DeregisterAction(Action* actionPtr);

    // Update all active actions; remove expired ones
    void Update();

    void Cleanup() {
        actions.clear();
    }

   private:
    GameLogic() = default;
    ~GameLogic() = default;
    GameLogic(const GameLogic&) = delete;
    GameLogic& operator=(const GameLogic&) = delete;

    std::vector<std::unique_ptr<Action>> actions;
};
