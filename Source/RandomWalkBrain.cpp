#include "RandomWalkBrain.h"

#include "Directions.h"
#include "base/Random.h"

USING_NS_AX;

Vec2 RandomWalkBrain::chooseDirection(const BomberGrid& grid, const Vec2& pos)
{
    std::vector<Vec2> dirs;
    for (const auto& dir : DIRECTIONS) {
        if (grid.isSafe(pos + dir * GridTile::SIZE))
            dirs.push_back(dir);
    }

    if (dirs.empty())
        return Vec2::ZERO;

    return dirs[random(0ul, dirs.size() - 1)];
}
