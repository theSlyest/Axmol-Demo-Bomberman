#pragma once

#include "EnemyBrain.h"

// Picks a random free neighboring tile each time.
class RandomWalkBrain : public EnemyBrain
{
public:
    ax::Vec2 chooseDirection(const BomberGrid& grid, const ax::Vec2& pos) override;
};
