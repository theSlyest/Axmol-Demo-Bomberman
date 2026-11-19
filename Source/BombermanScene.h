#pragma once

#include "2d/Label.h"
#include "2d/Layer.h"
#include "2d/Scene.h"
#include <memory>

#include "BomberControls.h"
#include "BomberGame.h"
#include "BomberGrid.h"
#include "BomberPlayer.h"

class BombermanScene : public ax::Scene
{
public:
    BombermanScene() : ax::Scene() {};

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(BombermanScene);

    void update(float dt) override;

    void reset();

private:
    const ax::Vec2 GRID_OFFSET = ax::Vec2(168.0f, 26.0f);
    const float UI_FONT_SIZE = 24.0f;
    const float GAME_OVER_SIZE = 32.0f;
    const float HELP_SIZE = 18.0f;
    const float UI_OFFSET = 32.0f;

    ax::Label* _scoreLbl = nullptr;
    ax::Label* _endScoreLbl = nullptr;
    ax::Label* _timeLbl = nullptr;
    ax::Label* _livesLbl = nullptr;
    ax::LayerColor* _endLayer = nullptr;

    BomberGrid* _grid = nullptr;
    BomberPlayer* _player = nullptr;
    std::vector<BomberPlayer*> _enemies;

    BomberControls* _controls = nullptr;
    std::unique_ptr<BomberGame> _game;

    void initScore(const ax::Vec2& origin, const ax::Size& size);
    void initTime(const ax::Vec2& origin, const ax::Size& size);
    void initLives(const ax::Vec2& origin, const ax::Size& size);
    void initGrid(const ax::Vec2& origin);
    void initEndLayer();
    void initPlayer();
    void initEnemies();
    void initControls();
    void initGame();
};
