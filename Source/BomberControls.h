#pragma once

#include <functional>
#include <map>

#include "2d/Node.h"
#include "EventListenerKeyboard.h"

// Translates keyboard events into game actions.
// It knows nothing about the grid, the player or the UI: the scene reads the
// held direction and reacts to the callbacks.
class BomberControls : public ax::Node
{
public:
    bool init() override;

    CREATE_FUNC(BomberControls);

    // Fired when a direction key goes down and no other direction key is held.
    std::function<void(const ax::Vec2&)> onDirectionPressed;
    std::function<void()> onBomb;
    std::function<void()> onRestart;

    // Direction currently held, or Vec2::ZERO.
    const ax::Vec2& getDirection() const { return _direction; }

    // Forget the held direction, e.g. when the game restarts.
    void clearDirection() { _direction = ax::Vec2::ZERO; }

private:
    ax::Vec2 _direction;

    void onKeyPressed(ax::EventKeyboard::KeyCode key);
    void onKeyReleased(ax::EventKeyboard::KeyCode key);
};
