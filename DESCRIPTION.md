# Bomberman

A minimal Bomberman built with Axmol. It covers basic gameplay only: no power-ups, no level progression.

## Gameplay

- Move with the **arrow keys**, drop a bomb with **Space**, restart with **R**.
- You have **3 lives** and **120 seconds**.
- A bomb explodes after 2 seconds. The blast reaches 2 tiles in each direction, destroys walls, stops at steel, and sets off other bombs it touches.
- You can have one bomb on the grid at a time.
- Touching an enemy or standing in fire costs a life. You are briefly invincible after respawning.
- Each enemy killed by a blast gives 100 points. Kill all three to win.
- The game ends when you win, lose all lives, or the timer reaches 0.

## What this example shows

| Concept | Where |
| --- | --- |
| Scene setup and wiring the parts together | `BombermanScene` |
| Frame loop with `update(dt)` | `BombermanScene::update`, `BomberGame::update` |
| Keyboard input (`EventListenerKeyboard`) | `BomberControls` |
| Game rules kept separate from the scene, reporting changes through callbacks | `BomberGame` |
| HUD labels and a game-over overlay as their own nodes | `BomberHud`, `BomberGameOverLayer` |
| Grid-locked movement (move tile to tile, turn only at tile centres) | `BomberGame::updatePlayer`, `BomberGame::updateEnemy` |
| Swappable enemy AI (strategy pattern) | `EnemyBrain`, `RandomWalkBrain` |
| Inheritance for shared behavior (`BomberActor` → `BomberPlayer`, `BomberEnemy`) | `BomberActor` |
| Procedural drawing with `DrawNode`, no image assets | `GridTile`, `BomberPlayer`, `BomberEnemy`, `BomberBomb` |
| Tile state machine (`SAFE`, `WALL`, `STEEL`, `BOMB`) and timers | `GridTile`, `BomberGrid::update` |
| Chain-reaction explosions, with the blast range as a constant | `BomberGrid::detonate`, `BomberGrid::BLAST_RANGE` |
| Grid coordinates vs. pixel positions kept inside the grid | `BomberGrid::coordsToPosition`, `positionToCoords`, `neighborPosition`, `getTileAtCenter` |
| Reference counting (`retain`/`release`) for objects shared between nodes | `GridTile::setBomb`, `BomberBomb` |

## Architecture

The scene only builds nodes and connects them. Input, rules and presentation do not know about each other.

```
BombermanScene          creates the nodes, wires everything, forwards update(dt)
 ├─ BomberControls      keyboard -> held direction + onDirectionPressed / onBomb / onRestart
 ├─ BomberGame          rules and state; calls onScoreChanged / onLivesChanged /
 │   │                  onTimeChanged / onGameOver
 │   └─ EnemyBrain      decides where an enemy goes next
 ├─ BomberHud           score, time and lives labels
 ├─ BomberGameOverLayer overlay shown when the game ends
 └─ BomberGrid          tiles, bombs and explosions (with the player and enemies as children)
```

Each frame the scene passes the held direction from `BomberControls` to `BomberGame::update`. The game updates the world and notifies the scene, which updates the HUD or shows the overlay.

### The grid owns the geometry

`BomberGrid` is the only place that knows the tile size. Game code and enemy brains ask it questions instead of doing the math:

```cpp
BomberGrid::neighborPosition(pos, direction);  // position of the next tile
grid.isSafe(pos);                              // can something walk here?
grid.getTileAtCenter(actor->getPosition());    // tile under an actor
```

Changing `GridTile::SIZE` or `BomberGrid::BLAST_RANGE` changes the game without touching the rules.

### Try it: change how enemies behave

Enemies pick their next direction through `EnemyBrain`. To change their behavior, implement it:

```cpp
class ChaseBrain : public EnemyBrain
{
public:
    explicit ChaseBrain(const BomberPlayer* target) : _target(target) {}

    ax::Vec2 chooseDirection(const BomberGrid& grid, const ax::Vec2& pos) override
    {
        // Pick the free direction that gets closest to _target->getPosition().
    }

private:
    const BomberPlayer* _target;
};
```

Then pass it to `BomberGame` in `BombermanScene::initGame`:

```cpp
_game = std::make_unique<BomberGame>(_grid, _player, _enemies, std::make_unique<ChaseBrain>(_player));
```

## Source layout

- `Source/BombermanScene.*`: creates the nodes and connects them.
- `Source/BomberControls.*`: keyboard handling. `Directions.h` holds the four grid directions.
- `Source/BomberGame.*`: score, lives, time, movement, bombs, fire, win and lose.
- `Source/EnemyBrain.h`, `Source/RandomWalkBrain.*`: the enemy decision interface and its default.
- `Source/BomberHud.*`, `Source/BomberGameOverLayer.*`: the on-screen labels and the end overlay.
- `Source/BomberGrid.*`: builds the 13x11 map, runs bombs and explosions, and converts between tile coordinates and positions so other code never does tile-size arithmetic.
- `Source/GridTile.*`: one cell of the map with its state, fire overlay and timers.
- `Source/BomberActor.*`: shared walking behavior (direction, destination, speed, spawn invincibility).
- `Source/BomberPlayer.*`, `Source/BomberEnemy.*`: the blue player (can drop a bomb) and the green enemies.
- `Source/BomberBomb.*`: the bomb sprite, linked back to the player who dropped it.
