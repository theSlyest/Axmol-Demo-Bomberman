#pragma once

#include "2d/DrawNode.h"
#include "BomberPlayer.h"

class BomberBomb : public ax::DrawNode
{
public:
    static const float SIZE;

    virtual bool init(BomberPlayer* player);
    
    // implement the "static create()" method manually
    static BomberBomb* create(BomberPlayer* player);

    void releaseBomber();

private:
    static const ax::Color4F COLOR;

    BomberPlayer* _player = nullptr;
};
