#include <iostream>

#include <cassert>

#include "Exceptions.h"
#include "Game.h"

using namespace std;
using namespace mtm;

void doCreate(Game& game)
{
    // {t} {health} {ammo} {range} {power} {pos[0]} {pos[1]}
    char char_type;
    CharacterType type;
    int health, ammo, range, power, x_pos, y_pos;
    cin >> char_type >> health >> ammo >> range >> power >> x_pos >> y_pos;
    if (tolower(char_type) == 's')
        type = SOLDIER;
    else if (tolower(char_type) == 'm')
        type = MEDIC;
    else
        type = SNIPER;
    Team team = isupper(char_type) ? POWERLIFTERS : CROSSFITTERS;
    shared_ptr<Character> temp = game.makeCharacter(type, team, health, ammo, range, power);
    game.addCharacter(GridPoint(y_pos, x_pos), temp);
}

void doAttack(Game& game)
{
    int x_src, y_src, x_dst, y_dst;
    cin >> x_src >> y_src >> x_dst >> y_dst;
    game.attack(GridPoint(y_src, x_src), GridPoint(y_dst, x_dst));
}

void doMove(Game& game)
{
    int x_src, y_src, x_dst, y_dst;
    cin >> x_src >> y_src >> x_dst >> y_dst;
    game.move(GridPoint(y_src, x_src), GridPoint(y_dst, x_dst));
}

void doReload(Game& game)
{
    int xpos, ypos;
    cin >> xpos >> ypos;
    game.reload(GridPoint(ypos, xpos));
}

int main()
{
    int width = 0;
    int height = 0;
    cin >> width >> height;
    Game game(height, width);
    int line = 2; // we start from line 2, because first line is the width and height
    while(!cin.eof())
    {
        char command = 0;
        try
        {
            cout << "executing line: " << line++ << endl;
            cin >> command;
            switch (command)
            {
                case 'c':
                    doCreate(game);
                    break;
                case 'i':
                    cout << "isOver: " << game.isOver(nullptr) << endl;
                    break;
                case 'a':
                    doAttack(game);
                    break;
                case 'm':
                    doMove(game);
                    break;
                case 'r':
                    doReload(game);
                    break;
                default:
                    break;
            }
            //cout << game << endl;
        }
        catch(mtm::Exception const& e)
        {
            cout << e.what() << endl;
        }
    }
}
