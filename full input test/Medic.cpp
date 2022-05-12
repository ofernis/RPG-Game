#include "Medic.h"

using namespace mtm;

Medic::Medic(units_t health, units_t ammo, units_t range, units_t power, Team team, CharacterType type) :
             Character(health, ammo, range, power, team, MOVEMENT_RANGE,RELOAD_AMMOUNT, ATTACK_AMMO_COST),
             type(MEDIC)
{}

Medic::Medic(const Medic &other) :
             Character(other), type(other.type)
{}

void Medic::reload()
{
    this->ammo = this->ammo + Medic::RELOAD_AMMOUNT;
}

void Medic::checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                          const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    if (src_coordinates == dst_coordinates)
    {
        throw IllegalTarget();
    }
}

void Medic::checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    int grid_difference = GridPoint::distance(src_coordinates, dst_coordinates);
    if (grid_difference > this->range)
    {
        throw OutOfRange();
    }
}

void Medic::attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                     const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    checkAttackLegal(board, src_coordinates, dst_coordinates);
    std::shared_ptr<Character> target_character = board.at(dst_coordinates);
    if (target_character->isEnemy(*this))
    {
        this->ammo -= 1;
        target_character->takeDamage(power);
        if (target_character->isDead())
        {
            board.erase(board.find(dst_coordinates));
        }
    }
    else
    {
        target_character->takeDamage(power*(-1));
    }
}

CharacterType Medic::getType()
{
    return this->type;
}

std::shared_ptr<Character> Medic::clone() const
{
    std::shared_ptr<Character> ptr(new Medic(*this));
    return ptr;
}

