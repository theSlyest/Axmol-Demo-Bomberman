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

    initScore(origin, visibleSize);
    initTime(origin, visibleSize);
    initLives(origin, visibleSize);
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
    _endLayer->setVisible(false);
    _game->reset();
}

void BombermanScene::initScore(const Vec2 &origin, const Size &size)
{
    _scoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _scoreLbl->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _scoreLbl->setPosition(origin.x + UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_scoreLbl);
}

void BombermanScene::initTime(const Vec2 &origin, const Size &size)
{
    _timeLbl = Label::createWithTTF("Time: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _timeLbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _timeLbl->setPosition(origin.x + size.width / 2.0f, origin.y + size.height - UI_OFFSET);
    addChild(_timeLbl);
}

void BombermanScene::initLives(const Vec2 &origin, const Size &size)
{
    _livesLbl = Label::createWithTTF("Lives: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _livesLbl->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _livesLbl->setPosition(origin.x + size.width - UI_OFFSET, origin.y + size.height - UI_OFFSET);
    addChild(_livesLbl);
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
    _endLayer = LayerColor::create(Color4B(0, 0, 0, 128));
    _endLayer->setVisible(false);
    addChild(_endLayer);

    Size size = _endLayer->getContentSize();
    auto gameOver = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", GAME_OVER_SIZE);
    gameOver->setPosition(size.width / 2.0f, size.height / 2.0f);
    _endLayer->addChild(gameOver);

    _endScoreLbl = Label::createWithTTF("Score: 0", "fonts/arial.ttf", UI_FONT_SIZE);
    _endScoreLbl->setPosition(size.width / 2.0f, size.height / 2.0f - 36.0f);
    _endLayer->addChild(_endScoreLbl);

    auto restart = Label::createWithTTF("Press R to Restart", "fonts/arial.ttf", HELP_SIZE);
    restart->setPosition(size.width / 2.0f, size.height / 2.0f - 72.0f);
    _endLayer->addChild(restart);
}

void BombermanScene::initPlayer()
{
    _player = BomberPlayer::create();
    _grid->addChild(_player);
}

void BombermanScene::initEnemies()
{
    for (int i = 0; i < 3; ++i) {
        auto enemy = BomberPlayer::create(true);
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
    _game->onScoreChanged = [this](const int score) { _scoreLbl->setString("Score: " + std::to_string(score)); };
    _game->onTimeChanged  = [this](const int time)  { _timeLbl->setString("Time: " + std::to_string(time)); };
    _game->onLivesChanged = [this](const int lives) { _livesLbl->setString("Lives: " + std::to_string(lives)); };
    _game->onGameOver = [this] {
        _endScoreLbl->setString(_scoreLbl->getString());
        _endLayer->setVisible(true);
    };
}
