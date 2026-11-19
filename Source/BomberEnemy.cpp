#include "BomberEnemy.h"

#include "GridTile.h"

USING_NS_AX;

const Color4F BomberEnemy::COLOR(0.33f, 0.79f, 0.21f, 1.0f);

bool BomberEnemy::init()
{
    if (!initActor(SPEED))
        return false;

    constexpr float radius = SIZE / 2.0f;
    drawSolidCircle(Vec2(GridTile::SIZE / 2.0f, GridTile::SIZE / 2.0f), radius, 0.0f, radius * M_PI, COLOR);

    return true;
}
