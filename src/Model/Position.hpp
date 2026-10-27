#pragma once

namespace Baba {

struct Position {
    int x = 0;
    int y = 0;

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position &other) const {
        return !(*this == other);
    }

    bool operator<(const Position &other) const {
        if (y != other.y) return y < other.y;
        return x < other.x;
    }

    Position operator+(const Position &other) const {
        return {x + other.x, y + other.y};
    }
};

}
