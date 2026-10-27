# Baba Is You — C++20

A small recreation of *Baba Is You*, built with C++20 and SFML 3. The idea is simple: the rules are objects on the board, so moving a word can change how the whole level works.

`BABA IS YOU` makes Baba controllable. `WALL IS STOP` makes walls solid. Break that sentence apart and the walls stop blocking you. The interesting part of this project is making movement, collisions, and victory conditions follow the current rules instead of hardcoding them for each object.

There are four built-in levels on a 15 × 10 board, with movable text, chain pushing, a pull action, water interactions, undo, restart, and sound feedback.

## How the engine works

### A small rule interpreter

[`Board::ParseRules()`](src/Model/Board.cpp) scans the grid horizontally and vertically for `NOUN IS PROPERTY` sentences. It rebuilds a `std::map<EntityType, std::set<Property>>` that the movement and collision code consults at runtime. Several properties can apply to the same object type, and text blocks are pushable by default.

The supported properties are `YOU`, `WIN`, `STOP`, `PUSH`, and `SINK`. This keeps object identity separate from behavior: a wall can become controllable, and a flag does not have to be the goal.

### Recursive movement and overlapping entities

Each cell stores a `std::vector<Entity>`, allowing several objects to share a position. That matters for detecting when a controlled object reaches a winning object or enters water.

Pushing follows a recursive chain: `CanMoveInto()` calls `MoveEntity()` for a pushable object ahead, continuing until the chain reaches free space or an obstacle. Controlled positions are sorted in the direction of travel before normal movement, so the front of a group is processed first.

### MVC and the Observer pattern

The code separates three responsibilities:

| Component | Responsibility |
| --- | --- |
| [`Board`](src/Model/Board.hpp) | Grid state, rules, movement, collisions, history, and win/loss checks |
| [`GameView`](src/View/GameView.hpp) | SFML sprites, interface buttons, result banners, and sounds |
| [`GameController`](src/Controller/GameController.cpp) | Translating keyboard events into model actions |

The model has no SFML dependency. The view implements [`IObserver`](src/Model/IObserver.hpp), and the board sends change notifications after movement, undo, and restart. The view uses these notifications to reset its sound playback flags without putting audio logic inside the model.

### Undo through state snapshots

Before a movement attempt, the board copies its grid into a history stack. Undo restores the latest snapshot and parses the rules again, so both object positions and their resulting behavior are restored. Restart uses a separate initial snapshot.

This is a straightforward use of value semantics and a Memento-style approach. Full copies are easy to reason about on a small board, though they use more memory than storing only the changes between turns. There is no redo stack.

### C++ design choices

- **Strongly typed enums** represent entity types, properties, and directions.
- **Operator overloading** gives positions comparison and addition operations, while `Board::operator[]` provides cell access.
- **Runtime polymorphism** connects the board to observers through a virtual interface. The board keeps non-owning observer pointers; the view is created and registered in `main()`.
- **A singleton asset manager** holds named textures, fonts, and sound buffers in maps. Textures and fonts are moved into storage, and the manager exposes resources through const references.
- **C++20 library features**, including `std::set::contains` and `std::erase_if`, simplify property checks and entity removal. `std::optional` is used for SFML events and sound objects.

## Build and run

You need CMake **3.14 or newer**, Git, and a **C++20** compiler. CMake fetches **SFML 3.0.0** on the first configuration, so that step needs an internet connection. Linux systems also need the development packages required by SFML's graphics and audio backends.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

Run from the repository root so the relative `assets/` paths resolve:

```sh
# Linux / macOS, or a single-configuration build
./build/BabaIsYou
```

```powershell
# Windows with Visual Studio
.\build\Release\BabaIsYou.exe
```

CMake also copies `assets/` beside the executable, so launching from that directory works too. On systems with GNU Make, the included `make` shortcut configures, builds, and launches the game.

## Controls

| Input | Action |
| --- | --- |
| Arrow keys / physical WASD keys | Move controlled objects |
| Ctrl + movement key | Move while pulling a pushable object from behind |
| Z | Undo the last movement attempt |
| R | Restart the current level |
| N | Cycle through the four levels |
| Escape | Close the game |
| Mouse | Use the undo and restart buttons |

## Project layout

```text
src/
  Model/       Board state, entities, rules, positions, observer interface
  View/        Rendering, interface, sound, asset manager
  Controller/  Keyboard input handling
  main.cpp     Level definitions and application loop
assets/        Textures, font, and sound effects
CMakeLists.txt
Makefile
```

## Scope and next steps

This is a focused learning project rather than a complete reproduction of the original game. Rules support a single noun and property; noun transformations and operators such as `AND` or `NOT` are not implemented. Levels are defined in C++, movement has no animation, and progress is not saved between sessions.

Useful next steps would be loading levels from files, adding tests around movement and rule changes, and making pull interactions follow the same sink processing as normal movement. A differential undo history would also be an interesting way to reduce memory use.
