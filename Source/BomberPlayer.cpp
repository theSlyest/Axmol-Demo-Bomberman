#include "BomberPlayer.h"

#include "GridTile.h"

USING_NS_AX;

const Color4F BomberPlayer::COLOR(0.28f, 0.36f, 0.94f, 1.0f);

bool BomberPlayer::init()
{
    if (!initActor(SPEED))
        return false;

    constexpr float offset = (GridTile::SIZE - SIZE) / 2.0f;
    drawSolidRect(Vec2(offset, offset), Vec2(GridTile::SIZE - offset, GridTile::SIZE - offset), COLOR);

    return true;
}
