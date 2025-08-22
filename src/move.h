#pragma once

#include "action.h"
#include "movable.h"

/* Move action - moves a Movable actor left or right when performed. */
class Move : public Action {
   public:
    enum class Direction { Left, Right };

    Move(Movable& target, Direction dir, float customSpeed = 0.0f)
        : Action(target), moveDir(dir), customSpeed(customSpeed) {}
    ~Move();
    virtual void Perform(float delta) override;

   private:
    Direction moveDir;
    float customSpeed;  // optional override for move speed magnitude
};
