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
| Scene setup, HUD labels, game-over overlay | `BombermanScene` |
| Frame loop with `update(dt)` | `BombermanScene::update` |
| Keyboard input (`EventListenerKeyboard`) | `BombermanScene::initKeyboardListener` |
| Grid-locked movement (move tile to tile, turn only at tile centres) | `updatePlayer`, `updateEnemy` |
| Simple enemy AI (pick a random free direction at each tile) | `chooseDirection` |
| Procedural drawing with `DrawNode`, no image assets | `GridTile`, `BomberPlayer`, `BomberBomb` |
| Tile state machine (`SAFE`, `WALL`, `STEEL`, `BOMB`) and timers | `GridTile`, `BomberGrid::update` |
| Chain-reaction explosions | `BomberGrid::detonate` |
| Reference counting (`retain`/`release`) for objects shared between nodes | `GridTile::setBomb`, `BomberBomb` |

## Source layout

- `Source/BombermanScene.*`: game rules, input, HUD, win/lose, reset.
- `Source/BomberGrid.*`: builds the 13x11 map and runs bombs and explosions.
- `Source/GridTile.*`: one cell of the map with its state, fire overlay and timers.
- `Source/BomberPlayer.*`: the player and the enemies (`create(true)` makes an enemy).
- `Source/BomberBomb.*`: the bomb sprite, linked back to the player who dropped it.
