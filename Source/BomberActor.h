#pragma once

#include "2d/DrawNode.h"

// Anything that walks on the grid tile by tile: the player and the enemies.
// An actor moves in a direction toward a destination tile centre.
class BomberActor : public ax::DrawNode
{
public:
    static constexpr float SIZE = 36.0f;
    static constexpr float OFFSET = 6.0f;
    static constexpr float SPAWN_DELAY = 0.55f;

    void setDirection(const ax::Vec2& direction) { _direction = direction; }
    ax::Vec2 getDirection() const { return _direction; }

    void setDestination(const ax::Vec2& dest) { _destination = dest; }
    ax::Vec2 getDestination() const { return _destination; }

    bool hasReachedDestination(const ax::Vec2& nextPos) const;

    float getSpeed() const { return _speed; }

    void setInvincible(bool invincible) { _remaining = invincible ? SPAWN_DELAY : 0.0f; }
    bool isInvincible() const { return _remaining > 0.0f; }

    void update(float dt) override;

protected:
    // Subclasses call this from init() with their speed, then draw themselves.
    bool initActor(float speed);

private:
    ax::Vec2 _direction;
    ax::Vec2 _destination;

    float _speed = 0;
    float _remaining = 0;
};
