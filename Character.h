#ifndef CHARACTER_H
#define CHARACTER_H

#include "Auxiliaries.h"
#include "Exceptions.h"
#include <math.h>
#include <memory>
#include <map>
#include <vector>

namespace mtm {

    struct GridCompare
    {
        /* 
         * @brief a grid compare operator - checks if the first point's coordinates smaller than the other
         * 
         * @param point1
         * @param point2
         */
        bool operator()(const GridPoint& point1, const GridPoint& point2) const
        {
            if (point1.row != point2.row)
            {
                return (point1.row < point2.row);
            }
            return (point1.col < point2.col);
        }
    };

    //Character is an abstact class - three heirs: Soldier, Medic, Sniper
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

        /* 
         * @brief constructor method of Character
         *
         * @param health
         * @param ammo
         * @param range
         * @param power
         * @param team
         * @param movement_range
         * @param reloard_amount
         * @param ammo_cost
         */
        Character(units_t health, units_t ammo, units_t range,
                    units_t power, Team team, units_t movement_range,
                    units_t reload_amount, units_t ammo_cost);

        /* 
         * @brief copy constructor method of Character
         *
         * @param other
         */
        Character(const Character &other);

        /* 
         * @brief destructor method of Character
         */
        virtual ~Character() {};

        /* 
         * @brief operator= of Character
         *
         * @param other
         * 
         * @return a new Character after assigning from the other Character
         */
        Character &operator=(const Character &other);

        /* 
         * @brief reload ammo to the Character
         */
        void reload();

        /* 
         * @brief check if given arguments are legal
         *
         * @param health
         * @param ammo
         * @param range
         * @param power
         */
        void checkArgumentsLegal(units_t health, units_t ammo, units_t range, units_t power);

        /* 
         * @brief check if a move from source coordinates to destination coordinates is legal 
         *
         * @param src_coordinates
         * @param dst_coordinates
         */
        void checkMoveLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /* 
         * @brief check if a character attack is legal 
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        virtual void checkAttackLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /* 
         * @brief check if a target character to attack is legal 
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        virtual void checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0;
        
        /* 
         * @brief check if attack range is legal 
         *
         * @param src_coordinates
         * @param dst_coordinates
         */
        virtual void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0 ;

        /* 
         * @brief check if a given character is an emeny
         *
         * @param other
         * 
         * @return true if the other character is an enemy, false otherwise
         */
        bool isEnemy(const Character &other);
        
        /* 
         * @brief check if a character is out of ammo
         */
        virtual void checkOutOfAmmo();

        /* 
         * @brief taking damage after being attacked 
         *
         * @param power
         */
        void takeDamage(units_t power);

        /* 
         * @brief check if a character is dead
         *
         * @return true if the character is dead, false otherwise
         */
        bool isDead();

        /* 
         * @brief perform an attack with the character on the board of the Game - pure virtual
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        virtual void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                            const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0;

        /* 
         * @brief clones a character - pure virtual
         *
         * @return a new Character after cloning it
         */
        virtual std::shared_ptr<Character> clone() const = 0;

        /* 
         * @brief get character type - pure virtual
         *
         * @return the character type
         */
        virtual CharacterType getType() = 0;

        /* 
         * @brief get character team
         *
         * @return the character team
         */
        Team getTeam();

    };

}

#endif //CHARACTER_H
