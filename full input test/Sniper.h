#ifndef SNIPER_H
#define SNIPER_H

#include "Character.h"

namespace mtm{

    class Sniper: public Character
    {
    private:
        CharacterType type;
        const int MOVEMENT_RANGE = 4;
        const int RELOAD_AMMOUNT = 2;
        const int ATTACK_AMMO_COST = 1;
        int successful_hits;
    public:
        Sniper(units_t health, units_t ammo, units_t range,
        units_t power, Team team, CharacterType type);

        Sniper(const Sniper &other);

        ~Sniper() = default;

        Sniper &operator=(const Sniper &other) = default;

        void reload();

        void checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                           const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                    const GridPoint &src_coordinates, const GridPoint &dst_coordinates) override;

        CharacterType getType();

        std::shared_ptr<Character> clone() const;

    };
}


#endif //SNIPER_H
