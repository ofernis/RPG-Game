#ifndef SOLDIER_H
#define SOLDIER_H

#include "Character.h"
#include <map>

namespace mtm {

    class Soldier : public Character {
    private:
        CharacterType type;
        static const int MOVEMENT_RANGE = 3;
        static const int RELOAD_AMMOUNT = 3;
        static const int ATTACK_AMMO_COST = 1;
    public:

        /* 
         * @brief constructor method of Soldier
         *
         * @param health
         * @param ammo
         * @param range
         * @param power
         * @param team
         * @param type
         */
        Soldier(units_t health, units_t ammo, units_t range,
                units_t power, Team team, CharacterType type);

        /* 
         * @brief copy constructor method of Soldier
         *
         * @param other
         */
        Soldier(const Soldier &other);

        /* 
         * @brief default destructor method of Soldier
         */
        ~Soldier() = default;

        /* 
         * @brief default operator= of Soldier
         *
         * @param other
         */
        Soldier &operator=(const Soldier &other) = default;

        /* 
         * @brief reload ammo to Soldier
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
         * @brief perform an attack with the Soldier on the board of the Game
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                            const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /* 
         * @brief removes dead characters from the gameboard
         *
         * @param board
         * @param dead_characters
         */
        void removeDeadCharacters(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                                        std::vector<GridPoint> dead_characters);

        /* 
         * @brief perform an area of effect (AOE) attack with the Soldier on the board of the Game
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */    
        void areaOfEffectAttack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates);
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

#endif //SOLDIER_H
