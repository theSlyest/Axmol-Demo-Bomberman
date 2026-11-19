#pragma once

#include "2d/Label.h"
#include "2d/Layer.h"

// Translucent "GAME OVER" overlay with the final score and a restart hint.
class BomberGameOverLayer : public ax::LayerColor
{
public:
    bool init() override;

    CREATE_FUNC(BomberGameOverLayer);

    void show(int score);
    void hide() { setVisible(false); }

private:
    static constexpr float TITLE_SIZE = 32.0f;
    static constexpr float SCORE_SIZE = 24.0f;
    static constexpr float HELP_SIZE = 18.0f;

    ax::Label* _scoreLbl = nullptr;

    ax::Label* addLabel(const char* text, float fontSize, float offsetY);
};
