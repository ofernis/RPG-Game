#ifndef MEDIC_H
#define MEDIC_H

#include "Character.h"

namespace mtm {

    class Medic : public Character {
    private:
        CharacterType type;
        static const int MOVEMENT_RANGE = 5;
        static const int RELOAD_AMMOUNT = 5;
        static const int ATTACK_AMMO_COST = 1;
    public:

        /* 
         * @brief constructor method of Medic
         *
         * @param health
         * @param ammo
         * @param range
         * @param power
         * @param team
         * @param type
         */
        Medic(units_t health, units_t ammo, units_t range,
              units_t power, Team team, CharacterType type);

        /* 
         * @brief copy constructor method of Medic
         *
         * @param other
         */
        Medic(const Medic &other);

        /* 
         * @brief default destructor method of Medic
         */
        ~Medic() = default;

        /* 
         * @brief default operator= of Medic
         *
         * @param other
         */
        Medic &operator=(const Medic &other) = default;

        /* 
         * @brief reload ammo to Medic
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
                              const mtm::GridPoint &src_coordinates, const mtm::GridPoint &dst_coordinates);

        /* 
         * @brief check if attack range is legal - throw exception if not
         *
         * @param src_coordinates
         * @param dst_coordinates
         */
        void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        /* 
         * @brief perform an attack with the Medic on the board of the Game
         *
         * @param board
         * @param src_coordinates
         * @param dst_coordinates
         */
        void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
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


#endif //MEDIC_H
