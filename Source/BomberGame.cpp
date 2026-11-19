#include "BomberGame.h"

#include <algorithm>

#include "Directions.h"
#include "RandomWalkBrain.h"

USING_NS_AX;

BomberGame::BomberGame(BomberGrid* grid, BomberPlayer* player, const std::vector<BomberEnemy*>& enemies,
                       std::unique_ptr<EnemyBrain> brain)
    : _grid(grid), _player(player), _enemies(enemies),
      _brain(brain ? std::move(brain) : std::make_unique<RandomWalkBrain>())
{
}

void BomberGame::reset()
{
    _grid->reset();
    respawnPlayer();
    _player->setInvincible(false);
    _player->clearBomb();
    resetEnemies();
    setLives(START_LIVES);
    setTime(START_TIME);
    _elapsed = 0.0f;
    setScore(0);
    _over = false;
}

void BomberGame::update(const float dt, const Vec2& keyDirection)
{
    if (_over)
        return;

    updateTime(dt);
    _grid->update(dt);
    updatePlayer(dt, keyDirection);
    for (const auto enemy : _enemies)
        updateEnemy(dt, enemy);
}

void BomberGame::dropBomb() const
{
    if (!_over)
        tryBomb(_player);
}

void BomberGame::setScore(const int score)
{
    _score = score;
    if (onScoreChanged)
        onScoreChanged(score);
}

void BomberGame::setTime(const int time)
{
    _time = time;
    if (onTimeChanged)
        onTimeChanged(time);
}

void BomberGame::setLives(const int lives)
{
    _lives = lives;
    if (onLivesChanged)
        onLivesChanged(lives);
}

void BomberGame::endGame()
{
    _over = true;
    if (onGameOver)
        onGameOver();
}

void BomberGame::updatePlayer(const float dt, const Vec2& keyDirection)
{
    _player->update(dt);

    if (const Vec2& direction = _player->getDirection(); direction != Vec2::ZERO) {
        const Vec2 nextPos = _player->getPosition() + direction * _player->getSpeed() * dt;
        if (_player->hasReachedDestination(nextPos)) {
            const Vec2 dest = _player->getDestination();
            if (direction == keyDirection) {
                const Vec2 nextDest = BomberGrid::neighborPosition(dest, direction);
                if (_grid->isSafe(nextDest)) {
                    _player->setPosition(nextPos);
                    _player->setDestination(nextDest);
                }
                else {
                    _player->setPosition(dest);
                    _player->setDirection(Vec2::ZERO);
                }
            }
            else {
                _player->setPosition(dest);
                const Vec2 nextDest = BomberGrid::neighborPosition(dest, keyDirection);
                if (keyDirection == Vec2::ZERO || !_grid->isSafe(nextDest))
                    _player->setDirection(Vec2::ZERO);
                else {
                    _player->setDestination(nextDest);
                    _player->setDirection(keyDirection);
                }
            }
        }
        else {
            _player->setPosition(nextPos);
        }
    }

    if (checkBurnt(_player) || checkEnemyContact())
        loseLife();
}

void BomberGame::movePlayer(const Vec2 &direction) const
{
    const Vec2& oldDirection = _player->getDirection();
    if (oldDirection == Vec2::ZERO) {
        const Vec2 dest = BomberGrid::neighborPosition(_player->getPosition(), direction);
        if (_grid->isSafe(dest)) {
            _player->setDestination(dest);
            _player->setDirection(direction);
        }
    }
    else if (oldDirection + direction == Vec2::ZERO) {
        _player->setDestination(BomberGrid::neighborPosition(_player->getDestination(), direction));
        _player->setDirection(direction);
    }
}

void BomberGame::updateEnemy(const float dt, BomberEnemy *enemy)
{
    if (!enemy->isVisible())
        return;

    const Vec2& direction = enemy->getDirection();

    if (direction != Vec2::ZERO) {
        const Vec2 nextPos = enemy->getPosition() + direction * enemy->getSpeed() * dt;
        if (enemy->hasReachedDestination(nextPos)) {
            const Vec2 dest = enemy->getDestination();
            const Vec2 nextDirection = _brain->chooseDirection(*_grid, dest);
            if (direction == nextDirection) {
                const Vec2 nextDest = BomberGrid::neighborPosition(dest, direction);
                if (_grid->isSafe(nextDest)) {
                    enemy->setPosition(nextPos);
                    enemy->setDestination(nextDest);
                }
                else {
                    enemy->setPosition(dest);
                    enemy->setDirection(Vec2::ZERO);
                }
            }
            else {
                enemy->setPosition(dest);
                const Vec2 nextDest = BomberGrid::neighborPosition(dest, nextDirection);
                if (nextDirection == Vec2::ZERO || !_grid->isSafe(nextDest))
                    enemy->setDirection(Vec2::ZERO);
                else {
                    enemy->setDestination(nextDest);
                    enemy->setDirection(nextDirection);
                }
            }
        }
        else
            enemy->setPosition(nextPos);
    }
    else {
        const Vec2 nextDirection = _brain->chooseDirection(*_grid, enemy->getPosition());
        enemy->setDestination(BomberGrid::neighborPosition(enemy->getPosition(), nextDirection));
        enemy->setDirection(nextDirection);
    }

    if (checkBurnt(enemy)) {
        setScore(_score + REWARD);
        enemy->setVisible(false);

        if (std::ranges::none_of(_enemies, [](const BomberEnemy* e) { return e->isVisible(); }))
            endGame();
    }
}

void BomberGame::updateTime(const float dt)
{
    _elapsed += dt;
    if (_elapsed >= 1.0f) {
        setTime(_time - 1);

        if (_time == 0)
            endGame();

        _elapsed -= 1.0f;
    }
}

void BomberGame::tryBomb(BomberPlayer* player) const
{
    if (player->hasBomb())
        return;

    const auto tile = _grid->getTileAtCenter(player->getPosition());
    if (tile->getState() != TileState::SAFE)
        return;

    if (tile->isOnFire())
        _grid->detonate(tile);
    else {
        const auto bomb = BomberBomb::create(player);
        bomb->setPosition(tile->getPosition());
        _grid->addChild(bomb);
        tile->setBomb(bomb);
        player->dropBomb();
    }
}

bool BomberGame::checkBurnt(const BomberActor *actor) const
{
    if (actor->isInvincible())
        return false;

    const auto tile1 = _grid->getTile(actor->getPosition() + Vec2(BomberActor::OFFSET, BomberActor::OFFSET));
    if (tile1->isOnFire())
        return true;

    const auto tile2 = _grid->getTile(actor->getPosition() + Vec2(GridTile::SIZE - BomberActor::OFFSET, GridTile::SIZE - BomberActor::OFFSET));
    if (tile2 != tile1 && tile2->isOnFire())
        return true;

    return false;
}

bool BomberGame::checkEnemyContact() const
{
    if (_player->isInvincible())
        return false;

    const Vec2 pos = _player->getPosition();
    for (const auto enemy : _enemies) {
        if (!enemy->isVisible())
            continue;

        const Vec2 delta = enemy->getPosition() - pos;
        if (std::abs(delta.x) < BomberActor::SIZE && std::abs(delta.y) < BomberActor::SIZE)
            return true;
    }

    return false;
}

void BomberGame::respawnPlayer() const
{
    _player->setPosition(_grid->getTilePosition(1, 1));
    _player->setDirection(Vec2::ZERO);
    _player->setDestination(_player->getPosition());
}

void BomberGame::loseLife()
{
    setLives(_lives - 1);
    if (_lives == 0)
        endGame();
    else {
        respawnPlayer();
        _player->setInvincible(true);
    }
}

void BomberGame::resetEnemies() const
{
    _enemies[0]->setPosition(_grid->getTilePosition(1, 9));
    _enemies[1]->setPosition(_grid->getTilePosition(11, 1));
    _enemies[2]->setPosition(_grid->getTilePosition(11, 9));
    for (const auto& enemy : _enemies) {
        enemy->setDirection(Vec2::ZERO);
        enemy->setVisible(true);
    }
}
