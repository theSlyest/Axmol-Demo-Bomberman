#include "BomberControls.h"

#include "Directions.h"
#include "EventDispatcher.h"

USING_NS_AX;

namespace {
const Vec2* directionFor(const EventKeyboard::KeyCode key)
{
    switch (key) {
        case EventKeyboard::KeyCode::KEY_UP_ARROW:    return &DIR_UP;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:  return &DIR_DOWN;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: return &DIR_RIGHT;
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:  return &DIR_LEFT;
        default:                                      return nullptr;
    }
}
}

bool BomberControls::init()
{
    if (!Node::init())
        return false;

    const auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](const EventKeyboard::KeyCode key, Event*) { onKeyPressed(key); };
    listener->onKeyReleased = [this](const EventKeyboard::KeyCode key, Event*) { onKeyReleased(key); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _direction = Vec2::ZERO;

    return true;
}

void BomberControls::onKeyPressed(const EventKeyboard::KeyCode key)
{
    if (const Vec2* direction = directionFor(key)) {
        // The first direction key held wins until it is released.
        if (_direction != Vec2::ZERO)
            return;

        _direction = *direction;
        if (onDirectionPressed)
            onDirectionPressed(_direction);
        return;
    }

    if (key == EventKeyboard::KeyCode::KEY_SPACE && onBomb)
        onBomb();
    else if (key == EventKeyboard::KeyCode::KEY_R && onRestart)
        onRestart();
}

void BomberControls::onKeyReleased(const EventKeyboard::KeyCode key)
{
    if (const Vec2* direction = directionFor(key); direction && _direction == *direction)
        _direction = Vec2::ZERO;
}
