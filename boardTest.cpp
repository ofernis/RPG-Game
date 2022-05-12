#include "Game.h"
#include "Character.h"
#include <iostream>

using namespace mtm;
using namespace std;

main ()
{
    Game* game = new Game(5,6);
    cout << game->isOver() << endl;
    
    return 0;
}