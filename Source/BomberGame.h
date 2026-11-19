#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "BomberGrid.h"
#include "BomberEnemy.h"
#include "BomberPlayer.h"
#include "EnemyBrain.h"

// The rules of the game: movement, bombs, fire, lives, score and time.
// It works on nodes it is given but does not create them, and it never touches
// the UI: the scene listens to the callbacks and renders the state.
class BomberGame
{
public:
    static constexpr int START_LIVES = 3;
    static constexpr int START_TIME = 120;
    static constexpr int REWARD = 100;

    // `brain` drives the enemies; the default is RandomWalkBrain.
    BomberGame(BomberGrid* grid, BomberPlayer* player, const std::vector<BomberEnemy*>& enemies,
               std::unique_ptr<EnemyBrain> brain = nullptr);

    // Notifications for the view.
    std::function<void(int)> onScoreChanged;
    std::function<void(int)> onLivesChanged;
    std::function<void(int)> onTimeChanged;
    std::function<void()> onGameOver;

    void reset();

    // keyDirection is the direction currently held by the player.
    void update(float dt, const ax::Vec2& keyDirection);

    // Player actions.
    void movePlayer(const ax::Vec2& direction) const;
    void dropBomb() const;

    [[nodiscard]] bool isOver() const { return _over; }
    [[nodiscard]] int getScore() const { return _score; }

private:
    int _score = 0;
    int _time  = 0;
    int _lives = 0;
    float _elapsed = 0;
    bool _over = true;

    BomberGrid* _grid = nullptr;
    BomberPlayer* _player = nullptr;
    std::vector<BomberEnemy*> _enemies;
    std::unique_ptr<EnemyBrain> _brain;

    void setScore(int score);
    void setTime(int time);
    void setLives(int lives);

    void updatePlayer(float dt, const ax::Vec2& keyDirection);
    void updateEnemy(float dt, BomberEnemy* enemy);
    void updateTime(float dt);


    void tryBomb(BomberPlayer* player) const;
    bool checkBurnt(const BomberActor* actor) const;
    [[nodiscard]] bool checkEnemyContact() const;

    void respawnPlayer() const;
    void resetEnemies() const;
    void loseLife();
    void endGame();
};
