#include "Soldier.h"
#include <memory>
#include <map>

using namespace mtm;

Soldier::Soldier(units_t health, units_t ammo, units_t range, units_t power, Team team, CharacterType type):
                 Character(health, ammo, range, power, team, MOVEMENT_RANGE, RELOAD_AMMOUNT, ATTACK_AMMO_COST),
                 type(SOLDIER)
{}

Soldier::Soldier(const Soldier &other) :
        Character(other), type(other.type)
{}

void Soldier::checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                      const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    if (src_coordinates.col != dst_coordinates.col && src_coordinates.row != dst_coordinates.row)
    {
        throw IllegalTarget();
    }
}

void Soldier::checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    int grid_difference = GridPoint::distance(src_coordinates, dst_coordinates);
    if (grid_difference > this->range)
    {
        throw OutOfRange();
    }
}

void Soldier::removeDeadCharacters(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                std::vector<GridPoint> dead_characters)
{
    for (GridPoint current : dead_characters)
    {
        board.erase(current);
    }
}

void Soldier::areaOfEffectAttack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    std::vector<GridPoint> dead_characters;
    int AOE_RANGE = (int)ceil((double)this->range / 3.0);
    int AOE_POWER = (int)ceil((double)this->power / 2.0);

    
    std::map<GridPoint, std::shared_ptr<Character>, GridCompare>::const_iterator iterator;
    for (iterator = board.begin(); board.find(iterator->first) != board.end(); iterator++)
    {
        GridPoint current_point = iterator->first;
        if (GridPoint::distance(current_point, dst_coordinates) <= AOE_RANGE &&
            GridPoint::distance(current_point, dst_coordinates) > 0)
        {
            std::shared_ptr<Character> target_character = iterator->second;
            if (target_character->isEnemy(*this))
            {
                target_character->takeDamage(AOE_POWER);
                if (target_character->isDead())
                {
                    dead_characters.push_back(iterator->first);
                }
            }
        }
    }
    removeDeadCharacters(board ,dead_characters);

    

    
    
    
}

void Soldier::attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                     const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    checkAttackLegal(board, src_coordinates, dst_coordinates);
    this->ammo -= 1;
    std::shared_ptr<Character> target_character;
    if (board.find(dst_coordinates) != board.end())
    {
        target_character = board.at(dst_coordinates);
        if (target_character->isEnemy(*this))
        {
            target_character->takeDamage(power);
            if (target_character->isDead())
            {
                board.erase(board.find(dst_coordinates));
            }
        }
    }
    areaOfEffectAttack(board, src_coordinates, dst_coordinates);
}

CharacterType Soldier::getType()
{
    return this->type;
}

std::shared_ptr<Character> Soldier::clone() const
{
    std::shared_ptr<Character> ptr(new Soldier(*this));
    return ptr;
}
