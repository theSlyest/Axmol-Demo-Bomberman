#include "BomberGameOverLayer.h"

USING_NS_AX;

bool BomberGameOverLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 128)))
        return false;

    setVisible(false);

    addLabel("GAME OVER", TITLE_SIZE, 0.0f);
    _scoreLbl = addLabel("Score: 0", SCORE_SIZE, -36.0f);
    addLabel("Press R to Restart", HELP_SIZE, -72.0f);

    return true;
}

Label* BomberGameOverLayer::addLabel(const char* text, const float fontSize, const float offsetY)
{
    const auto label = Label::createWithTTF(text, "fonts/arial.ttf", fontSize);
    label->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f + offsetY);
    addChild(label);
    return label;
}

void BomberGameOverLayer::show(const int score)
{
    _scoreLbl->setString("Score: " + std::to_string(score));
    setVisible(true);
}
