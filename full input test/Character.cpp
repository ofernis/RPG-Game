#include "Character.h"

using namespace mtm;

Character::Character(units_t health, units_t ammo, units_t range, units_t power, Team team,
                     units_t movement_range, units_t reload_amount, units_t ammo_cost) :
                     health(health), ammo(ammo), range(range), power(power), team(team)
{
    ///NEED TO CHECK EXPECTIONS HERE AND THROW THE CORRECT SHIT
}

Character::Character(const Character &other) :
                    health(other.health), ammo(other.ammo), range(other.ammo),
                    power(other.power), team(other.team)
{}

Character& Character::operator=(const Character &other) {
    if (this == &other)
    {
        return *this;
    }

    this->health = other.health;
    this->ammo = other.ammo;
    this->range = other.range;
    this->power = other.power;
    this->team = other.team;
    return *this;
}

void Character::reload()
{
    this->ammo += this->reload_amount;
}

bool Character::isEnemy(const Character &other)
{
    return (this->team != other.team);
}

void Character::checkOutOfAmmo()
{
    if (this->ammo <= 0)
    {
        throw OutOfAmmo();
    }
}

void Character::checkMoveLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    int grid_difference = GridPoint::distance(src_coordinates, dst_coordinates);
    if (grid_difference > Character::movement_range)
    {
        throw MoveTooFar();
    }
}

void Character::checkAttackLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                 const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    checkAttackRangeLegal(src_coordinates, dst_coordinates);
    checkOutOfAmmo();
    checkTargetLegal(board, src_coordinates, dst_coordinates);
}

void Character::takeDamage(const units_t power)
{
    this->health -= power;
}

bool Character::isDead()
{
    return (this->health <= 0);
}

Team Character::getTeam()
{
    return this->team;
}