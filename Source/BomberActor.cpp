#include "BomberActor.h"

#include "GridTile.h"

USING_NS_AX;

bool BomberActor::initActor(const float speed)
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(GridTile::SIZE, GridTile::SIZE));
    _speed = speed;

    return true;
}

bool BomberActor::hasReachedDestination(const Vec2& nextPos) const
{
    if (_direction.x > 0.0f && nextPos.x >= _destination.x)
        return true;

    if (_direction.x < 0.0f && nextPos.x <= _destination.x)
        return true;

    if (_direction.y > 0.0f && nextPos.y >= _destination.y)
        return true;

    if (_direction.y < 0.0f && nextPos.y <= _destination.y)
        return true;

    return false;
}

void BomberActor::update(const float dt)
{
    if (_remaining > 0.0f)
        _remaining -= dt;
}
