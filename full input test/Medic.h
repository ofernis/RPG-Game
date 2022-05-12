#ifndef MEDIC_H
#define MEDIC_H

#include "Character.h"

namespace mtm {

    class Medic : public Character {
    private:
        CharacterType type;
        const int MOVEMENT_RANGE = 5;
        const int RELOAD_AMMOUNT = 5;
        const int ATTACK_AMMO_COST = 1;
    public:
        Medic(units_t health, units_t ammo, units_t range,
              units_t power, Team team, CharacterType type);

        Medic(const Medic &other);

        ~Medic() = default;

        Medic &operator=(const Medic &other) = default;

        void reload();

        void checkTargetLegal(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                              const mtm::GridPoint &src_coordinates, const mtm::GridPoint &dst_coordinates);

        void checkAttackRangeLegal(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        void attack(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>& board,
                    const GridPoint &src_coordinates, const GridPoint &dst_coordinates);

        

        CharacterType getType();

        std::shared_ptr<Character> clone() const;
    };
}


#endif //MEDIC_H
