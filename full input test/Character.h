#ifndef CHARACTER_H
#define CHARACTER_H

#include "Auxiliaries.h"
#include "Exceptions.h"
#include <math.h>
#include <memory>
#include <map>

namespace mtm {

    struct GridCompare
    {
        bool operator()(const GridPoint& point1, const GridPoint& point2) const
        {
            if(!(point1.row == point2.row))
            {
                return (point1.row < point2.row);
            }
            return (point1.col < point2.col);
        }
    };

    class Character {
    protected:
        units_t health;
        units_t ammo;
        units_t range;
        units_t power;
        Team team;
        units_t movement_range;
        units_t reload_amount;
        units_t ammo_cost;
        
    public:
        Character(units_t health, units_t ammo, units_t range,
                    units_t power, Team team, units_t movement_range,
                    units_t reload_amount, units_t ammo_cost);

        Character(const Character &other);

        virtual ~Character() {};

        Character &operator=(const Character &other);

        void reload();

        void checkMoveLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        virtual void checkAttackLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        virtual void checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0;
        
        virtual void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0 ;

        bool isEnemy(const Character &other);
        
        virtual void checkOutOfAmmo();

        bool isEnemy(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                const GridPoint &dst_coordinates);

        void takeDamage(units_t power);

        bool isDead();

        virtual void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                            const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0;

        virtual std::shared_ptr<Character> clone() const = 0;

        virtual CharacterType getType() = 0;

        Team getTeam();

    };

}

#endif //CHARACTER_H
