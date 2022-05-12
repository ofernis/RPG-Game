#include "Exceptions.h" //ifndef??

using namespace mtm;

const char* IllegalArgument::what() const noexcept
{
    return "A game related error has occurred: IllegalArgument";
}

const char* IllegalCell::what() const noexcept
{
    return "A game related error has occurred: IllegalCell";
}

const char* CellEmpty::what() const noexcept
{
    return "A game related error has occurred: CellEmpty";
}

const char* MoveTooFar::what() const noexcept
{
    return "A game related error has occurred: MoveTooFar";
}

const char* CellOccupied::what() const noexcept
{
    return "A game related error has occurred: CellOccupied";
}

const char* OutOfRange::what() const noexcept
{
    return "A game related error has occurred: OutOfRange";
}

const char* OutOfAmmo::what() const noexcept
{
    return "A game related error has occurred: OutOfAmmo";
}

const char* IllegalTarget::what() const noexcept
{
    return "A game related error has occurred: IllegalTarget";
}