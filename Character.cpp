#include "Auxiliaries.h"
#include "Character.h"

using namespace mtm;

Character::Character(units_t health, units_t ammo, units_t range, units_t power, Team team,
                     units_t movement_range, units_t reload_amount, units_t ammo_cost) :
                     team(team), movement_range(movement_range), reload_amount(reload_amount), ammo_cost(ammo_cost)
{
    checkArgumentsLegal(health, ammo, range, power);
    this->health = health;
    this->ammo = ammo;
    this->range = range;
    this->power = power;
}

Character::Character(const Character &other) :
                    health(other.health), ammo(other.ammo), range(other.ammo),power(other.power),
                    team(other.team), movement_range(other.movement_range), reload_amount(other.reload_amount), ammo_cost(other.ammo_cost)
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
    this->movement_range = other.movement_range;
    this->reload_amount = other.reload_amount;
    this->ammo_cost = other.ammo_cost;
    return *this;
}

void Character::reload()
{
    this->ammo += this->reload_amount;
}

void Character::checkArgumentsLegal(units_t health, units_t ammo, units_t range, units_t power)
{
    if (health <= 0 || ammo < 0 || range < 0 || power < 0)
    {
        throw IllegalArgument();
    }
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
    if (grid_difference > this->movement_range)
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