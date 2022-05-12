#include "Auxiliaries.h"

std::ostream &mtm::printGameBoard(std::ostream &os, const char *begin,
                                 const char *end, unsigned int width) {
    std::string delimiter = std::string(2 * width + 1, '*');
    const char *temp = begin;
    os << delimiter << std::endl;
    while (temp != end) {
        os << "|" << (*temp);
        ++temp;
        if ((temp - begin) % width == 0)
            os << "|" << std::endl;
    }
    os << delimiter;
    return os;
}