#pragma once

#include "BomberActor.h"

// A green circle that roams the grid; its direction comes from an EnemyBrain.
class BomberEnemy : public BomberActor
{
public:
    static constexpr float SPEED = 100.0f;

    bool init() override;

    CREATE_FUNC(BomberEnemy);

private:
    static const ax::Color4F COLOR;
};
