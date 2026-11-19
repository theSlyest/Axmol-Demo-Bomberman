#include "BomberGame.h"

#include <algorithm>

#include "Directions.h"
#include "base/Random.h"

USING_NS_AX;

BomberGame::BomberGame(BomberGrid* grid, BomberPlayer* player, const std::vector<BomberPlayer*>& enemies)
    : _grid(grid), _player(player), _enemies(enemies)
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
                const Vec2 nextDest = dest + direction * GridTile::SIZE;
                if (isSafe(nextDest)) {
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
                const Vec2 nextDest = dest + keyDirection * GridTile::SIZE;
                if (keyDirection == Vec2::ZERO || !isSafe(nextDest))
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
        const Vec2 dest = _player->getPosition() + direction * GridTile::SIZE;
        if (isSafe(dest)) {
            _player->setDestination(dest);
            _player->setDirection(direction);
        }
    }
    else if (oldDirection + direction == Vec2::ZERO) {
        _player->setDestination(_player->getDestination() + direction * GridTile::SIZE);
        _player->setDirection(direction);
    }
}

void BomberGame::updateEnemy(const float dt, BomberPlayer *enemy)
{
    if (!enemy->isVisible())
        return;

    const Vec2& direction = enemy->getDirection();

    if (direction != Vec2::ZERO) {
        const Vec2 nextPos = enemy->getPosition() + direction * enemy->getSpeed() * dt;
        if (enemy->hasReachedDestination(nextPos)) {
            const Vec2 dest = enemy->getDestination();
            const Vec2 nextDirection = chooseDirection(dest);
            if (direction == nextDirection) {
                const Vec2 nextDest = dest + direction * GridTile::SIZE;
                if (isSafe(nextDest)) {
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
                const Vec2 nextDest = dest + nextDirection * GridTile::SIZE;
                if (nextDirection == Vec2::ZERO || !isSafe(nextDest))
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
        const Vec2 nextDirection = chooseDirection(enemy->getPosition());
        enemy->setDestination(enemy->getPosition() + nextDirection * GridTile::SIZE);
        enemy->setDirection(nextDirection);
    }

    if (checkBurnt(enemy)) {
        setScore(_score + REWARD);
        enemy->setVisible(false);

        if (std::ranges::none_of(_enemies, [](const BomberPlayer* e) { return e->isVisible(); }))
            endGame();
    }
}

Vec2 BomberGame::chooseDirection(const Vec2& pos) const
{
    std::vector<Vec2> dirs;
    for (const auto& dir : DIRECTIONS) {
        if (isSafe(pos + dir * GridTile::SIZE))
            dirs.push_back(dir);
    }

    if (dirs.empty())
        return Vec2::ZERO;

    return dirs[random(0ul, dirs.size() - 1)];
}

bool BomberGame::isSafe(const Vec2& pos) const
{
    return _grid->getTile(pos)->getState() == TileState::SAFE;
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

    const auto tile = _grid->getTile(player->getPosition() + Vec2(GridTile::SIZE, GridTile::SIZE) / 2.0f);
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

bool BomberGame::checkBurnt(const BomberPlayer *player) const
{
    if (player->isInvincible())
        return false;

    const auto tile1 = _grid->getTile(player->getPosition() + Vec2(BomberPlayer::OFFSET, BomberPlayer::OFFSET));
    if (tile1->isOnFire())
        return true;

    const auto tile2 = _grid->getTile(player->getPosition() + Vec2(GridTile::SIZE - BomberPlayer::OFFSET, GridTile::SIZE -BomberPlayer::OFFSET));
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
        if (std::abs(delta.x) < BomberPlayer::SIZE && std::abs(delta.y) < BomberPlayer::SIZE)
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
