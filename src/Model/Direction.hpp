#pragma once
#include "Position.hpp"

namespace Baba {

enum class Direction { Up, Down, Left, Right, None };

inline Position GetDelta(const Direction dir) {
    switch (dir) {
    case Direction::Up:
        return {0, -1};
    case Direction::Down:
        return {0, 1};
    case Direction::Left:
        return {-1, 0};
    case Direction::Right:
        return {1, 0};
    default:
        return {0, 0};
    }
}

}
