#ifndef SNIPER_H
#define SNIPER_H

#include "Character.h"

namespace mtm{

    class Sniper: public Character
    {
    private:
        CharacterType type;
        static const int MOVEMENT_RANGE = 4;
        static const int RELOAD_AMMOUNT = 2;
        static const int ATTACK_AMMO_COST = 1;
        int successful_hits;
    public:

        /* 
         * @brief constructor method of Sniper
         *
         * @param health
         * @param ammo
         * @param range
         * @param power
         * @param team
         * @param type
         */
        Sniper(units_t health, units_t ammo, units_t range,
        units_t power, Team team, CharacterType type);

        /* 
         * @brief copy constructor method of Sniper
         *
         * @param other
         */
        Sniper(const Sniper &other);

        /* 
         * @brief default destructor method of Sniper
         */
        ~Sniper() = default;

        /* 
         * @brief default operator= of Sniper
         *
         * @param other
         */
        Sniper &operator=(const Sniper &other) = default;

        /* 
         * @brief reload ammo to Sniper
         */
        void reload();

        /* 
         * @brief check if a target is legal to attack - throw exception if not
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        void checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                           const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /* 
         * @brief check if attack range is legal - throw exception if not
         *
         * @param src_coordinates
         * @param dst_coordinates
         */
        void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /* 
         * @brief perform an attack with the Sniper on the board of the Game
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                    const GridPoint &src_coordinates, const GridPoint &dst_coordinates) override;

        /* 
         * @brief get character type - inherits from Character
         *
         * @return the character type
         */
        CharacterType getType();

        /* 
         * @brief clones the character
         *
         * @return a shared_ptr to the new character
         */
        std::shared_ptr<Character> clone() const;

    };
}


#endif //SNIPER_H
