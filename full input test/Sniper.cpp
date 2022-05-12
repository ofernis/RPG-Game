#include "Sniper.h"
#include <cmath>

using namespace mtm;

Sniper::Sniper(units_t health, units_t ammo, units_t range, units_t power, Team team, CharacterType type) :
               Character(health, ammo, range, power, team, MOVEMENT_RANGE, RELOAD_AMMOUNT, ATTACK_AMMO_COST),
               type(SNIPER), successful_hits(1)

{}

Sniper::Sniper(const Sniper &other) :
               Character(other), type(other.type), successful_hits(other.successful_hits)
{}


void Sniper::checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                          const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    std::shared_ptr<Character> target_character = board.at(dst_coordinates);
    if (!target_character->isEnemy(*this));
    {
        throw IllegalTarget();
    }
}

void Sniper::checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    int min_range = (int)ceil((double)this->range / 2.0);
    int grid_difference = GridPoint::distance(src_coordinates, dst_coordinates);
    if (grid_difference < min_range || grid_difference > this->range)
    {
        throw OutOfRange();
    }
}

void Sniper::attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                     const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    checkAttackLegal(board, src_coordinates, dst_coordinates);
    std::shared_ptr<Character> target_character = board.at(dst_coordinates);
    if (target_character->isEnemy(*this))
    {
        int current_power = (successful_hits % 3 == 0) ? power : power * 2;
        
        target_character->takeDamage(current_power);
        if (target_character->isDead())
        {
            board.erase(board.find(dst_coordinates));
        }
        this->ammo -= 1;
        this->successful_hits += 1;
    }
}

CharacterType Sniper::getType()
{
    return this->type;
}

std::shared_ptr<Character> Sniper::clone() const
{
    std::shared_ptr<Character> ptr(new Sniper(*this));
    return ptr;
}


