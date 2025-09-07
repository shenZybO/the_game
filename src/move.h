#pragma once

#include "types.h"
#include "action.h"
#include "movable.h"

/* Move action - moves a Movable actor left or right when performed. */
class Move : public Action {
   public:
    Move(Actor& target, GameTypes::Direction dir, float customSpeed = 0.0f)
        : Action(target), moveDir(dir), customSpeed(customSpeed) {}

    ~Move();
    
    void OnPerform(float delta) override;

   private:
    GameTypes::Direction moveDir;
    float customSpeed;  // optional override for move speed magnitude
};
