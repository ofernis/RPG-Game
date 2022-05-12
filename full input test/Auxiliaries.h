#ifndef HW3_AUXILIARIES_H
#define HW3_AUXILIARIES_H

#include <iostream>
#include <string>

#include <cmath>

namespace mtm {
    enum Team {
        POWERLIFTERS, CROSSFITTERS
    };
    enum CharacterType {
        SOLDIER, MEDIC, SNIPER
    };
    typedef int units_t;

    struct GridPoint {
        int row, col;

        GridPoint(int row, int col) : row(row), col(col) {}

        GridPoint(const GridPoint &other) = default;

        ~GridPoint() = default;

        GridPoint &operator=(const GridPoint &other) = default;

        bool operator==(const GridPoint &other) const {
            return this->row == other.row && this->col == other.col;
        }

        static int distance(const GridPoint &point1, const GridPoint &point2) {
            return std::abs(point1.row - point2.row)
                   + std::abs(point1.col - point2.col);
        }
    };


    std::ostream &printGameBoard(std::ostream &os, const char *begin,
                                 const char *end, unsigned int width);
}

#endif