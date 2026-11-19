#pragma once

#include "BomberActor.h"

// The human-controlled actor: a blue square that can drop a bomb.
class BomberPlayer : public BomberActor
{
public:
    static constexpr float SPEED = 200.0f;

    bool init() override;

    CREATE_FUNC(BomberPlayer);

    void dropBomb() { _hasBomb = true; }
    void clearBomb() { _hasBomb = false; }
    bool hasBomb() const { return _hasBomb; }

private:
    static const ax::Color4F COLOR;

    bool _hasBomb = false;
};
