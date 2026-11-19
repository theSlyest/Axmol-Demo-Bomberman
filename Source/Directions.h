#pragma once

#include <array>

#include "math/Vec2.h"

// The four grid directions, shared by the controls and the enemy AI.
inline constexpr ax::Vec2 DIR_UP(0, 1);
inline constexpr ax::Vec2 DIR_DOWN(0, -1);
inline constexpr ax::Vec2 DIR_RIGHT(1, 0);
inline constexpr ax::Vec2 DIR_LEFT(-1, 0);

inline constexpr std::array<ax::Vec2, 4> DIRECTIONS = { DIR_UP, DIR_DOWN, DIR_RIGHT, DIR_LEFT };
