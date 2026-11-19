# Axmol Demo: Bomberman

A small Bomberman built with [Axmol Engine](https://axmol.dev/) and C++, written as a learning resource. It covers basic grid-based bomb gameplay, and the code is split into small parts that are easy to read one at a time.

## Gameplay

- Move with the **arrow keys**, drop a bomb with **Space**, restart with **R**.
- You have **3 lives** and **120 seconds**.
- A bomb explodes after 2 seconds. The blast reaches 2 tiles in each direction, destroys walls, stops at steel, and sets off other bombs it touches.
- You can have one bomb on the grid at a time.
- Touching an enemy or standing in fire costs a life. You are briefly invincible after respawning.
- Each enemy killed by a blast gives 100 points. Kill all three to win.
- The game ends when you win, lose all lives, or the timer reaches 0.

There are no image assets: everything is drawn with `DrawNode`.

## Requirements

* A C++ development environment
* [CMake](https://cmake.org/) 3.22 or later
* [Axmol Engine](https://github.com/axmolengine/axmol) and the platform tools it requires

Refer to the [Axmol documentation](https://axmol.dev/manual/latest/) for installation and platform setup.

## Build and run

Clone the repository, then use the Axmol command-line tools:

```bash
git clone https://github.com/theSlyest/Axmol-Demo-Bomberman.git
cd Axmol-Demo-Bomberman
axmol run -p osx -a arm64    # build and launch on Apple Silicon macOS
```

Change `-p` (and `-a`) for your platform, for example `-p win32` or `-p linux`. To build without launching, use `axmol build` with the same options. The platform project folders (`proj.android`, `proj.ios_mac`, `proj.linux`, `proj.wasm`, `proj.win32`, `proj.winrt`) come from the Axmol project template.

## How the code is organized

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

See [DESCRIPTION.md](DESCRIPTION.md) for a table of the Axmol concepts the code demonstrates, the file layout, and an example of swapping the enemy AI. The commit history follows the refactor step by step, so it also works as a walkthrough.

## Topics covered

* Scene setup and node management
* Keyboard input
* Grid-based, tile-by-tile movement
* Tile state machines and timers
* Chain-reaction explosions
* Swappable enemy behavior (strategy pattern)
* Separating input, game rules and UI with callbacks
* Procedural drawing with `DrawNode`
* Reference counting (`retain` / `release`)

## Disclaimer

This project is intended for experimentation and education. It simplifies architecture, error handling and content in order to keep the concepts easy to follow.

## License

This repository is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

Third-party assets and dependencies may be subject to their own licenses.
