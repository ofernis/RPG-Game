#ifndef SOLDIER_H
#define SOLDIER_H

#include "Character.h"
#include <map>

namespace mtm {

    class Soldier : public Character {
    private:
        CharacterType type;
        const int MOVEMENT_RANGE = 3;
        const int RELOAD_AMMOUNT = 3;
        const int ATTACK_AMMO_COST = 1;
    public:
        Soldier(units_t health, units_t ammo, units_t range,
                units_t power, Team team, CharacterType type);

        Soldier(const Soldier &other);

        ~Soldier() = default;

        Soldier &operator=(const Soldier &other) = default;

        void reload();

        void checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                           const GridPoint &src_coordinates, const GridPoint &dst_coordinates) {};

        void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                            const GridPoint &src_coordinates, const GridPoint &dst_coordinates);
                            
        void areaOfEffectAttack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                   const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        CharacterType getType();

        std::shared_ptr<Character> clone() const;
    };
}

#endif //SOLDIER_H
