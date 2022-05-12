#ifndef GAME_H
#define GAME_H

#include <map>
#include "Auxiliaries.h"
#include "Character.h"
#include <memory>

namespace mtm {
    class Game {
    private:
        int height;
        int width;
        std::map<GridPoint, std::shared_ptr<Character>, GridCompare> board;

        void checkCoordinatesLegal(const GridPoint& coordinates) const;
        void checkCellOccupied(const GridPoint& coordinates) const;
        void checkCellEmpty(const GridPoint& coordinates) const;
        static void checkArgumentsLegal(units_t health, units_t ammo, units_t range, units_t power);
    public:
        Game(int height, int width);
        Game(const Game &other);
        ~Game() = default;
        Game& operator=(const Game& other);
        void addCharacter(const GridPoint& coordinates, std::shared_ptr<Character> character);
        static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team,
                                                        units_t health, units_t ammo, units_t range, units_t power);
        void move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);
        void attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);
        void reload(const GridPoint& coordinates);
        bool isOver(Team* winningTeam = NULL);
        int getWidth() const { return width; }
        std::string toString() const;
        friend std::ostream& operator<<(std::ostream& os, const Game& game);
    };
}

std::ostream& operator<<(std::ostream& os, const mtm::Game& game)
{
    std::string game_string = game.toString();
    return mtm::printGameBoard(os, &*game_string.begin(), &*game_string.end(), game.getWidth());
}

#endif //GAME_H
