#pragma once

#include "2d/Label.h"
#include "2d/Layer.h"
#include "2d/Scene.h"
#include "EventListenerKeyboard.h"
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
    const int START_LIVES = 3;
    const int START_TIME = 120;
    const int REWARD = 100;
    const std::map<ax::EventKeyboard::KeyCode, ax::Vec2> DIRECTIONS = {
        { ax::EventKeyboard::KeyCode::KEY_UP_ARROW,    ax::Vec2( 0,  1) },
        { ax::EventKeyboard::KeyCode::KEY_DOWN_ARROW,  ax::Vec2( 0, -1) },
        { ax::EventKeyboard::KeyCode::KEY_RIGHT_ARROW, ax::Vec2( 1,  0) },
        { ax::EventKeyboard::KeyCode::KEY_LEFT_ARROW,  ax::Vec2(-1,  0) }
    };

    int _score = 0;
    int _time  = 0;
    int _lives = 0;
    float _elapsed = 0;

    ax::Label* _scoreLbl = nullptr;
    ax::Label* _endScoreLbl = nullptr;
    ax::Label* _timeLbl = nullptr;
    ax::Label* _livesLbl = nullptr;
    ax::LayerColor* _endLayer = nullptr;

    BomberGrid* _grid = nullptr;
    BomberPlayer* _player = nullptr;
    std::vector<BomberPlayer*> _enemies;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::Vec2 _keyDirection;

    void initScore(const ax::Vec2& origin, const ax::Size& size);
    void initTime(const ax::Vec2& origin, const ax::Size& size);
    void initLives(const ax::Vec2& origin, const ax::Size& size);
    void initGrid(const ax::Vec2& origin);
    void initEndLayer();
    void initPlayer();
    void initEnemies();
    void initKeyboardListener();

    void setScore(int score);
    void setTime(int time);
    void setLives(int lives);

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);

    void updatePlayer(float dt);
    void movePlayer(const ax::Vec2& direction) const;

    void updateEnemy(float dt, BomberPlayer* enemy);
    ax::Vec2 chooseDirection(const ax::Vec2& pos) const;

    void updateTime(float dt);

    void tryBomb(BomberPlayer* player) const;
    bool checkBurnt(const BomberPlayer* player) const;
    bool checkEnemyContact() const;
    void respawnPlayer() const;

    void showGameOver();
    void loseLife();

    void resetEnemies() const;
};
