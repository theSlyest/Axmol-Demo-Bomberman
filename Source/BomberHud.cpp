#include "BomberHud.h"

USING_NS_AX;

BomberHud* BomberHud::create(const Size& size)
{
    if (auto* pRet = new (std::nothrow) BomberHud(); pRet && pRet->init(size))
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

bool BomberHud::init(const Size& size)
{
    if (!Node::init())
        return false;

    setContentSize(size);

    const float top = size.height - OFFSET;
    _scoreLbl = addLabel("Score: 0", Vec2::ANCHOR_TOP_LEFT, Vec2(OFFSET, top));
    _timeLbl  = addLabel("Time: 0", Vec2::ANCHOR_MIDDLE_TOP, Vec2(size.width / 2.0f, top));
    _livesLbl = addLabel("Lives: 0", Vec2::ANCHOR_TOP_RIGHT, Vec2(size.width - OFFSET, top));

    return true;
}

Label* BomberHud::addLabel(const char* text, const Vec2& anchor, const Vec2& position)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", FONT_SIZE);
    label->setAnchorPoint(anchor);
    label->setPosition(position);
    addChild(label);
    return label;
}

void BomberHud::setScore(const int score)
{
    _scoreLbl->setString("Score: " + std::to_string(score));
}

void BomberHud::setTime(const int time)
{
    _timeLbl->setString("Time: " + std::to_string(time));
}

void BomberHud::setLives(const int lives)
{
    _livesLbl->setString("Lives: " + std::to_string(lives));
}
