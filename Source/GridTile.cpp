#include "GridTile.h"

USING_NS_AX;

const std::map<TileState, Color4F> GridTile::COLORS {
    { TileState::SAFE, Color4F(0.0f, 0.0f, 0.0f, 1.0f) },
    { TileState::WALL, Color4F(0.60f, 0.40f, 0.05f, 1.0f) },
    { TileState::STEEL, Color4F(0.27f, 0.27f, 0.34f, 1.0f) },
};

const Color4F GridTile::BORDER_COLOR(0.73f, 0.73f, 0.66f, 1.0f);
const Color4F GridTile::FIRE_COLOR(0.92f, 0.34f, 0.29f, 1.0f);

// on "init" you need to initialize your instance
bool GridTile::init(const TileState state)
{
    if (!DrawNode::init())
        return false;

    setContentSize(Size(SIZE, SIZE));

    _state = state;
    drawState(state);

    const auto border = DrawNode::create();
    border->drawRect(Vec2::ZERO, Vec2(_contentSize), BORDER_COLOR, 1.0f);
    addChild(border);

    constexpr Vec2 offset(3.0f, 3.0f);
    _fire = DrawNode::create();
    _fire->setContentSize(_contentSize);
    _fire->drawSolidRect(offset, Vec2(_contentSize) - offset, FIRE_COLOR);
    _fire->setVisible(false);
    addChild(_fire);

    return true;
}

void GridTile::setState(const TileState state)
{
    _state = state;
    drawState(state);
}

void GridTile::setBomb(BomberBomb *bomb)
{
    _state = TileState::BOMB;
    _bomb = bomb;
    bomb->retain();
    _remaining = BOMB_DELAY;
}

void GridTile::releaseBomb()
{
    if (_bomb == nullptr)
        return;

    _bomb->releaseBomber();
    _bomb->release();
    _bomb->removeFromParent();
    _bomb = nullptr;
}

void GridTile::startFire()
{
    if (_state == TileState::BOMB)
       releaseBomb();

    setState(TileState::SAFE);
    _remaining = FIRE_DELAY;
    _fire->setVisible(true);
}

bool GridTile::updateState(const float dt)
{
    if (_state == TileState::BOMB || _fire->isVisible()) {
        _remaining -= dt;
        if (_remaining <= 0.0f) {
            if (_state == TileState::BOMB)
                return true;

            stopFire();
        }
    }

    return false;
}

void GridTile::stopFire() const
{
    _fire->setVisible(false);
}

void GridTile::drawState(const TileState state)
{
    clear();
    if (state == TileState::BOMB)
        drawSolidRect(Vec2::ZERO, Vec2(_contentSize), COLORS.at(TileState::SAFE));
    else
        drawSolidRect(Vec2::ZERO, Vec2(_contentSize), COLORS.at(state));
}

GridTile* GridTile::create(const TileState state)
{
    if (auto* pRet = new (std::nothrow) GridTile(); pRet && pRet->init(state))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}
