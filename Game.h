#ifndef GAME_H
#define GAME_H

#include "Auxiliaries.h"
#include "Character.h"
#include <map>
#include <memory>

namespace mtm {
    class Game {
    private:
        int height;
        int width;
        std::map<GridPoint, std::shared_ptr<Character>, GridCompare> board;

        /* 
         * @brief check if given game coordinates are legal, throw error if not
         * 
         * @param coordinates
         */
        void checkCoordinatesLegal(const GridPoint& coordinates) const;

        /* 
         * @brief check if given cell is occupied, throw error if not
         * 
         * @param coordinates
         */
        void checkCellOccupied(const GridPoint& coordinates) const;

        /* 
         * @brief check if given cell is empty, throw error if not
         * 
         * @param coordinates
         */
        void checkCellEmpty(const GridPoint& coordinates) const;
    public:

        /* 
         * @brief constructor method of Game
         *
         * @param height
         * @param width
         */
        Game(int height, int width);

        /* 
         * @brief copy constructor method of Game
         *
         * @param other
         */
        Game(const Game &other);

        /* 
         * @brief default destructor method of Game
         */
        ~Game() = default;

        /* 
         * @brief operator= of Game
         *
         * @param other
         * 
         * @return new Game after assign from other Game
         */
        Game& operator=(const Game& other);

        /* 
         * @brief add a new character to the game
         *
         * @param coordinates
         * @param character
         */
        void addCharacter(const GridPoint& coordinates, std::shared_ptr<Character> character);
        
        /* 
         * @brief make a new character
         *
         * @param type
         * @param team
         * @param health
         * @param ammo
         * @param range
         * @param power
         */
        static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team,
                                                        units_t health, units_t ammo, units_t range, units_t power);
        
        /* 
         * @brief move a character on the board of the Game
         *
         * @param src_coordinates
         * @param dst_coordinates
         */
        void move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);
        
        /* 
         * @brief perform an attack with a character on the board of the Game
         *
         * @param src_coordinates
         * @param dst_coordinates
         */
        void attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);
        
        /* 
         * @brief reload ammo to a character on the board of the Game
         *
         * @param coordinates
         */
        void reload(const GridPoint& coordinates);

        /* 
         * @brief check wether the game is over - determined by the remaining
         *        of only one team characters on the board
         *
         * @param winningTeam
         * 
         * @return ture if game is over, false otherwise
         */
        bool isOver(Team* winningTeam = NULL);

        /* 
         * @brief get the width of the gameboard
         * 
         * @return the width of the gameboard
         */
        int getWidth() const { return width; }

        /* 
         * @brief create a char string that represents the current gameboard
         * 
         * @return gameboard string
         */
        std::string toString() const;
        
        /* 
         * @brief a friend function of Game class, overloads the std::operator<<
         * 
         * @param os
         * @param game
         * 
         * @return output of gameboard to print
         */
        friend std::ostream& operator<<(std::ostream& os, const Game& game);
    };
    
}

#endif //GAME_H
