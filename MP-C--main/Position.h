#ifndef POSITION_H
#define POSITION_H
#include <cstdlib>

class Position {
public:
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    virtual ~Position() {}

    int manhattanDistance(const Position& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
    friend bool operator==(const Position& a, const Position& b) {
        return a.x == b.x && a.y == b.y;
    }
    friend bool operator!=(const Position& a, const Position& b) {
        return !(a == b);
    }
    friend Position operator+(const Position& a, const Position& b) {
        return Position(a.x + b.x, a.y + b.y);
    }
};
#endif