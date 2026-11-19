#pragma once

#include "2d/Node.h"
#include "GridTile.h"

using GridCoords = std::pair<int, int>;

class BomberGrid : public ax::Node
{
public:
    static constexpr int COLS = 13;
    static constexpr int ROWS = 11;
    static constexpr int BLAST_RANGE = 2; // tiles reached by an explosion in each direction

    bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(BomberGrid);

    // Tile coordinates <-> positions in the grid's local space.
    static ax::Vec2 coordsToPosition(const GridCoords& coords)
    {
        return { static_cast<float>(coords.first) * GridTile::SIZE, static_cast<float>(coords.second) * GridTile::SIZE };
    }
    static GridCoords positionToCoords(const ax::Vec2& position);

    // Position of the tile next to `position` in the given unit direction.
    static ax::Vec2 neighborPosition(const ax::Vec2& position, const ax::Vec2& direction)
    {
        return position + direction * GridTile::SIZE;
    }

    ax::Vec2 getTilePosition(const int col, const int row) const { return coordsToPosition({ col, row }); }
    TileState getTileState(const int col, const int row) const  { return _tiles[col][row]->getState(); }
    void setTileState(const int col, const int row, const TileState state) const { _tiles[col][row]->setState(state); }

    GridTile* getTile(const ax::Vec2& position) const;
    // Tile under the centre of a tile-sized node whose bottom-left corner is at `position`.
    GridTile* getTileAtCenter(const ax::Vec2& position) const;
    bool isSafe(const ax::Vec2& position) const { return getTile(position)->getState() == TileState::SAFE; }

    void reset() const;

    void detonate(int col, int row);
    void detonate(const GridTile* tile);

    void update(float dt) override;

private:

    const std::set<GridCoords> SAFE_COORDS {
        {1, 1}, {1, 2}, {2, 1},
        {1, 8}, {1, 9}, {2, 9},
        {10, 1}, {11, 1}, {11, 2},
        {10, 9}, {11, 8}, {11, 9}
    };

    std::vector<std::vector<GridTile*>> _tiles;

    bool tryFire(int col, int row);
};
