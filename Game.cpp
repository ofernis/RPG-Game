#include "Game.h"
#include "Soldier.h"
#include "Medic.h"
#include "Sniper.h"
#include "Exceptions.h"
#include <map>
#include <iostream>


namespace mtm
{

    void Game::checkCoordinatesLegal(const GridPoint &coordinates) const
    {
        if (coordinates.col < 0 || coordinates.col >= width ||
            coordinates.row < 0 || coordinates.row >= height)
        {
            throw IllegalCell();
        }
    }

    void Game::checkCellOccupied(const GridPoint &coordinates) const
    {
        if (board.find(coordinates) != board.end())
        {
            throw CellOccupied();
        }
    }

    void Game::checkCellEmpty(const GridPoint& coordinates) const
    {
        if (board.find(coordinates) == board.end())
        {
            throw CellEmpty();
        }
    }

    Game::Game(int height, int width) :
            height(height), width(width), board(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>())
    {
        if (height <= 0 || width <= 0)
        {
            throw IllegalArgument();
        }
    }

    Game::Game(const Game &other):
            height(other.height), width(other.width),
            board(std::map<GridPoint, std::shared_ptr<Character>, GridCompare>())
    {
        std::shared_ptr<Character> current_character;
        std::shared_ptr<Character>copy_character;
        std::map<GridPoint, std::shared_ptr<Character>, GridCompare>::const_iterator i;
        for (i = other.board.begin(); i != other.board.end(); i++)
        {
            current_character = i->second;
            copy_character = current_character->clone();
            board.insert(make_pair(i->first, copy_character));
        }
    }

    Game &Game::operator=(const Game &other)
    {
        if (this == &other)
        {
            return *this;
        }

        this->board.clear();
        this->height = other.height;
        this->width = other.width;
        this->board = std::map<GridPoint, std::shared_ptr<Character>, GridCompare>(); //changed

        std::shared_ptr<Character> current_character, character_copy;
        std::map<GridPoint, std::shared_ptr<Character>, GridCompare>::const_iterator i;
        for (i = other.board.begin(); i != other.board.end(); i++)
        {
            current_character = i->second;
            character_copy = current_character->clone();
            board.insert(make_pair(i->first,character_copy));
        }
        return *this;
    }

    void Game::addCharacter(const GridPoint &coordinates, std::shared_ptr<Character> character)
    {
        checkCoordinatesLegal(coordinates);
        checkCellOccupied(coordinates);
        board.insert(make_pair(coordinates, character));
    }

    std::shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team, units_t health, units_t ammo, units_t range, units_t power)
    {
        std::shared_ptr<Character> new_character;

        switch (type)
        {
        case SOLDIER:
            new_character = std::shared_ptr<Character> (new Soldier(health, ammo, range, power, team, type));
            break;
        case MEDIC:
            new_character = std::shared_ptr<Character> (new Medic(health, ammo, range, power, team, type));
            break;
        case SNIPER:
            new_character = std::shared_ptr<Character> (new Sniper(health, ammo, range, power, team ,type));;
            break;   
        default:
            break;
        }

        return new_character;
    }


    void Game::move(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
    {
        checkCoordinatesLegal(src_coordinates);
        checkCoordinatesLegal(dst_coordinates);
        checkCellEmpty(src_coordinates);

        std::shared_ptr<Character> character = board.at(src_coordinates);
        character->checkMoveLegal(src_coordinates, dst_coordinates);
        checkCellOccupied(dst_coordinates);
        
        board.insert(make_pair(dst_coordinates, character));
        board.erase(src_coordinates);
    }

    void Game::attack(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
    {
        checkCoordinatesLegal(src_coordinates);
        checkCoordinatesLegal(dst_coordinates);
        checkCellEmpty(src_coordinates);

        std::shared_ptr<Character> character = board.at(src_coordinates);
        character->attack(this->board, src_coordinates, dst_coordinates); 
    }

    void Game::reload(const GridPoint &coordinates)
    {
        checkCoordinatesLegal(coordinates);
        checkCellEmpty(coordinates);

        std::shared_ptr<Character> character = board.at(coordinates);
        character->reload();
    }

    std::string Game::toString() const
    {
        std::string game_string = "";
        char current_letter = '\0';
        std::shared_ptr<Character> current_character;

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {            
                try
                {   
                    current_character = board.at(GridPoint(i,j));

                    switch (current_character->getType())
                    {
                    case SOLDIER:
                        current_letter = 's';
                        break;
                    case MEDIC:
                        current_letter = 'm';
                        break;
                    case SNIPER:
                        current_letter = 'n';
                        break;   
                    default:
                        break;
                    }

                    if(current_character->getTeam() == POWERLIFTERS)
                    {
                        current_letter = toupper(current_letter);
                    }
                }
                catch (std::out_of_range& e)
                {
                    current_letter = ' ';
                }

                game_string.push_back(current_letter);   
            }
        }
        return game_string;
    }

    std::ostream& operator<<(std::ostream& os, const Game& game)
    {
        std::string game_string = game.toString();
        printGameBoard(os, &*game_string.begin(), &*game_string.end(), game.getWidth());
        return os;
    }

    bool Game::isOver(Team* winningTeam)
    {
        bool is_crossfitter_found = false;
        bool is_powerlifter_found = false;

        std::map<GridPoint, std::shared_ptr<Character>, GridCompare>::const_iterator i;
        for(i = this->board.begin() ; i != this->board.end() ; i++)
        {
            std::shared_ptr<Character> current_character = i->second;

            if(current_character->getTeam() == CROSSFITTERS)
            {
                is_crossfitter_found = true;
            }
            if(current_character->getTeam() == POWERLIFTERS)
            {
                is_powerlifter_found = true;
            }
            if(is_powerlifter_found && is_crossfitter_found)
            {
                return false;
            }
        }

        if(is_powerlifter_found == false && is_crossfitter_found == false)
        {
            return false;
        }

        if (winningTeam == nullptr) {}
        else if(is_powerlifter_found == true && is_crossfitter_found == false)
        {
            *winningTeam = POWERLIFTERS;
            
        }
        else if(is_powerlifter_found == false && is_crossfitter_found == true)
        {
            *winningTeam = CROSSFITTERS;
        }
        return true;
    }
}