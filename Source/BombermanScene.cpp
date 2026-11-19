#include "BombermanScene.h"

#include "Director.h"

USING_NS_AX;

// on "init" you need to initialize your instance
bool BombermanScene::init()
{
    if (!Scene::init())
        return false;

    const auto visibleSize = _director->getVisibleSize();
    const Vec2 origin = _director->getVisibleOrigin();

    initHud(origin, visibleSize);
    initGrid(origin);
    initEndLayer();
    initControls();
    initGame();

    reset();
    scheduleUpdate();

    return true;
}

void BombermanScene::update(const float dt)
{
    _game->update(dt, _controls->getDirection());
}

void BombermanScene::reset()
{
    _controls->clearDirection();
    _endLayer->hide();
    _game->reset();
}

void BombermanScene::initHud(const Vec2 &origin, const Size &size)
{
    _hud = BomberHud::create(size);
    _hud->setPosition(origin);
    addChild(_hud);
}

void BombermanScene::initGrid(const Vec2 &origin)
{
    _grid = BomberGrid::create();
    _grid->setPosition(origin + GRID_OFFSET);
    addChild(_grid);
    initEnemies();
    initPlayer();
}

void BombermanScene::initEndLayer()
{
    _endLayer = BomberGameOverLayer::create();
    addChild(_endLayer);
}

void BombermanScene::initPlayer()
{
    _player = BomberPlayer::create();
    _grid->addChild(_player);
}

void BombermanScene::initEnemies()
{
    for (int i = 0; i < 3; ++i) {
        auto enemy = BomberEnemy::create();
        _grid->addChild(enemy);
        _enemies.push_back(enemy);
    }
}

void BombermanScene::initControls()
{
    _controls = BomberControls::create();
    _controls->onDirectionPressed = [this](const Vec2& direction) {
        if (!_game->isOver())
            _game->movePlayer(direction);
    };
    _controls->onBomb = [this] { _game->dropBomb(); };
    _controls->onRestart = [this] { reset(); };
    addChild(_controls);
}

void BombermanScene::initGame()
{
    _game = std::make_unique<BomberGame>(_grid, _player, _enemies);
    _game->onScoreChanged = [this](const int score) { _hud->setScore(score); };
    _game->onTimeChanged  = [this](const int time)  { _hud->setTime(time); };
    _game->onLivesChanged = [this](const int lives) { _hud->setLives(lives); };
    _game->onGameOver = [this] { _endLayer->show(_game->getScore()); };
}
