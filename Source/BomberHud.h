#pragma once

#include "2d/Label.h"
#include "2d/Node.h"

// Score, time and lives labels along the top of the screen.
class BomberHud : public ax::Node
{
public:
    // The HUD covers an area of the given size; place the node at the visible origin.
    static BomberHud* create(const ax::Size& size);

    bool init(const ax::Size& size);

    void setScore(int score);
    void setTime(int time);
    void setLives(int lives);

private:
    static constexpr float FONT_SIZE = 24.0f;
    static constexpr float OFFSET = 32.0f;

    ax::Label* _scoreLbl = nullptr;
    ax::Label* _timeLbl = nullptr;
    ax::Label* _livesLbl = nullptr;

    ax::Label* addLabel(const char* text, const ax::Vec2& anchor, const ax::Vec2& position);
};
