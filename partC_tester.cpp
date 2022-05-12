
#include <map>
#include <iostream>
#include <sstream>
#include <functional>
#include <cmath>
#include <cassert>


#include "Exceptions.h"
#include "Game.h"

using namespace mtm;
using std::cout;
using std::endl;
using std::string;


// Asserts that x is true
#define ASSERT_TEST(x) do{if(!(x)){ \
    cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<endl;\
    return false; }} while(false);

// Asserts that x causes no errors
#define ASSERT_NO_ERROR(x) do{try{x;}catch(mtm::Exception& e){cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received error "<<"\""<<e.what()<<"\""<<" while expecting no errors"<<endl; return false;}} while(false);

// Asserts that x causes this error (and no other error)
#define ASSERT_ERROR(x, error) do{ \
    try{x; cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received no error while expecting "<<#error<<endl; return false;}\
    catch(error& e){}\
    catch(const mtm::Exception& e){cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received error: "<<"\""<<e.what()<<"\" while expecting "<<"\""<<#error<<"\""<<endl; return false;}\
    } while(false);

// Asserts that x causes this error (and no other error) with message
#define ASSERT_ERROR_WITH_MESSAGE(x, error, message) do{ \
    try{x; cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received no error while expecting "<<#error<<endl; return false;}\
    catch(error& e){ASSERT_TEST(string(e.what()) == (message));}\
    catch(const mtm::Exception& e){cout<<"Failed assertion at line "<<__LINE__<<" in "<<__func__<<\
    ": received error: "<<"\""<<e.what()<<"\" while expecting "<<"\""<<#error<<"\""<<endl; return false;}\
    } while(false);

#define ADD_TEST(x) tests[#x]=x;

bool checkGameContainsPlayerAt(Game& game, GridPoint point){
    try{
        game.move(point, point);
    }
    catch(mtm::CellOccupied&){
        return true;
    }
    catch(...){
        return false;
    }
    return false;
}

bool testErrorNames(){

    std::shared_ptr<Character> testCharacter1 = Game::makeCharacter(CharacterType::SOLDIER, Team::POWERLIFTERS, 1, 0, 2, 0);
    std::shared_ptr<Character> testCharacter2 = Game::makeCharacter(CharacterType::MEDIC, Team::POWERLIFTERS, 1, 10, 2, 0);
    Game testGame (10,10);
    ASSERT_NO_ERROR(testGame.addCharacter(GridPoint(1,1), testCharacter1));
    ASSERT_NO_ERROR(testGame.addCharacter(GridPoint(3,1), testCharacter2));

    ASSERT_ERROR_WITH_MESSAGE(Game(0, 0), IllegalArgument, "A game related error has occurred: IllegalArgument");
    ASSERT_ERROR_WITH_MESSAGE(testGame.addCharacter(GridPoint(-1,-1), testCharacter1), IllegalCell, "A game related error has occurred: IllegalCell");
    ASSERT_ERROR_WITH_MESSAGE(testGame.move(GridPoint(0,0), GridPoint(0,0)), CellEmpty, "A game related error has occurred: CellEmpty");
    ASSERT_ERROR_WITH_MESSAGE(testGame.move(GridPoint(1,1), GridPoint(9,9)), MoveTooFar, "A game related error has occurred: MoveTooFar");
    ASSERT_ERROR_WITH_MESSAGE(testGame.move(GridPoint(1,1), GridPoint(1,1)), CellOccupied, "A game related error has occurred: CellOccupied");
    ASSERT_ERROR_WITH_MESSAGE(testGame.attack(GridPoint(1,1), GridPoint(3,3)), OutOfRange, "A game related error has occurred: OutOfRange");
    ASSERT_ERROR_WITH_MESSAGE(testGame.attack(GridPoint(1,1), GridPoint(1,2)), OutOfAmmo, "A game related error has occurred: OutOfAmmo");
    ASSERT_ERROR_WITH_MESSAGE(testGame.attack(GridPoint(3,1), GridPoint(3,1)), IllegalTarget, "A game related error has occurred: IllegalTarget");

    return true;

}

bool testConstructor(){

    ASSERT_ERROR(Game(0,0), IllegalArgument);
    ASSERT_ERROR(Game(0,1), IllegalArgument);
    ASSERT_ERROR(Game(1,0), IllegalArgument);
    ASSERT_ERROR(Game(-1,-1), IllegalArgument);

    return true;

}

bool testCopyConstructor(){

    int rows = 5;
    int cols = 8;

    Game g1 = Game(rows,cols);

    ASSERT_NO_ERROR(g1.addCharacter(GridPoint(2,2), Game::makeCharacter(CharacterType::SOLDIER, Team::POWERLIFTERS, 1, 0, 10, 0)));

    Game g2(g1);

    // Move from (2,2) to (2,1) in g1 only
    ASSERT_NO_ERROR(g1.move(GridPoint(2,2), GridPoint(2,1)));

    // Check that (2,2) is empty and that (2,1) is occupied in g1
    ASSERT_TEST(!checkGameContainsPlayerAt(g1, GridPoint(2,2)));
    ASSERT_TEST(checkGameContainsPlayerAt(g1, GridPoint(2,1)));
    
    // Check that (2,2) is occupied and that (2,1) is empty in g2
    ASSERT_TEST(checkGameContainsPlayerAt(g2, GridPoint(2,2)));
    ASSERT_TEST(!checkGameContainsPlayerAt(g2, GridPoint(2,1)));

    ASSERT_NO_ERROR(g1.reload(GridPoint(2,1)));

    ASSERT_NO_ERROR(g1.attack(GridPoint(2,1), GridPoint(2,1)));
    ASSERT_ERROR(g2.attack(GridPoint(2,2), GridPoint(2,2)), OutOfAmmo);

    return true;

}

bool testAssignemntOperator(){

    int rows = 5;
    int cols = 8;

    Game g1 = Game(rows,cols);

    ASSERT_NO_ERROR(g1.addCharacter(GridPoint(2,2), Game::makeCharacter(CharacterType::SOLDIER, Team::POWERLIFTERS, 1, 0, 10, 0)));

    Game g2 = Game(1,1);
    g2 = g1;

    // Move from (2,2) to (2,1) in g1 only
    ASSERT_NO_ERROR(g1.move(GridPoint(2,2), GridPoint(2,1)));

    // Check that (2,2) is empty and that (2,1) is occupied in g1
    ASSERT_TEST(!checkGameContainsPlayerAt(g1, GridPoint(2,2)));
    ASSERT_TEST(checkGameContainsPlayerAt(g1, GridPoint(2,1)));
    
    // Check that (2,2) is occupied and that (2,1) is empty in g2
    ASSERT_TEST(checkGameContainsPlayerAt(g2, GridPoint(2,2)));
    ASSERT_TEST(!checkGameContainsPlayerAt(g2, GridPoint(2,1)));

    ASSERT_NO_ERROR(g1.reload(GridPoint(2,1)));

    ASSERT_NO_ERROR(g1.attack(GridPoint(2,1), GridPoint(2,1)));
    ASSERT_ERROR(g2.attack(GridPoint(2,2), GridPoint(2,2)), OutOfAmmo);

    return true;

}

bool testAddCharacter(){

    int rows = 5;
    int cols = 8;

    Game game = Game(rows,cols);

    ASSERT_ERROR(game.addCharacter(GridPoint(-1,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 10, 0, 0, 0)), IllegalCell);
    ASSERT_ERROR(game.addCharacter(GridPoint(rows-1,cols), Game::makeCharacter(SOLDIER, POWERLIFTERS, 10, 0, 0, 0)), IllegalCell);
    ASSERT_ERROR(game.addCharacter(GridPoint(rows,cols-1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 10, 0, 0, 0)), IllegalCell);
    ASSERT_ERROR(game.addCharacter(GridPoint(rows,cols), Game::makeCharacter(SOLDIER, POWERLIFTERS, 10, 0, 0, 0)), IllegalCell);

    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 10, 0, 0, 0)));
    ASSERT_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 10, 0, 0, 0)), mtm::CellOccupied);

    return true;

}

bool testMakeCharacter(){

    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, 0, 0, 0, 0), IllegalArgument);
    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, 0, 1, 2, 3), IllegalArgument);
    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, -1, 1, 2, 3), IllegalArgument);

    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, -1), IllegalArgument);
    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, -1, 0), IllegalArgument);
    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, -1, 0, 0), IllegalArgument);
    ASSERT_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, -1, 0, 0, 0), IllegalArgument);

    ASSERT_NO_ERROR(Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0));

    return true;

}

bool testMove(){

    int rows = 20;
    int cols = 20;

    Game game = Game(rows,cols);

    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(-1,0)), IllegalCell);
    ASSERT_ERROR(game.move(GridPoint(-1,0), GridPoint(1,0)), IllegalCell);
    ASSERT_ERROR(game.move(GridPoint(-1,0), GridPoint(-1,0)), IllegalCell);
    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(rows,0)), IllegalCell);
    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(0,cols)), IllegalCell);
    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(rows,cols)), IllegalCell);

    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(0,0)), CellEmpty);

    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,5), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 0, 0)));

    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(5,5)), MoveTooFar);

    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(0,0)), mtm::CellOccupied);
    ASSERT_ERROR(game.move(GridPoint(0,0), GridPoint(1,1)), mtm::CellOccupied);
    ASSERT_ERROR(game.move(GridPoint(1,1), GridPoint(0,0)), mtm::CellOccupied);

    game = Game(rows,cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            ASSERT_NO_ERROR(game.addCharacter(GridPoint(i,j), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 0, 0)));
        }
    }
    GridPoint selected(9,9);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint::distance(GridPoint(i,j), selected) <= 3){
                ASSERT_ERROR(game.move(selected, GridPoint(i,j)), mtm::CellOccupied);
            }
            else{
                ASSERT_ERROR(game.move(selected, GridPoint(i,j)), MoveTooFar);
            }
        }
    }

    game = Game(rows,cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            ASSERT_NO_ERROR(game.addCharacter(GridPoint(i,j), Game::makeCharacter(MEDIC, POWERLIFTERS, 1, 1, 0, 0)));
        }
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint::distance(GridPoint(i,j), selected) <= 5){
                ASSERT_ERROR(game.move(selected, GridPoint(i,j)), mtm::CellOccupied);
            }
            else{
                ASSERT_ERROR(game.move(selected, GridPoint(i,j)), MoveTooFar);
            }
        }
    }

    game = Game(rows,cols);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            ASSERT_NO_ERROR(game.addCharacter(GridPoint(i,j), Game::makeCharacter(SNIPER, POWERLIFTERS, 1, 1, 0, 0)));
        }
    }
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint::distance(GridPoint(i,j), selected) <= 4){
                ASSERT_ERROR(game.move(selected, GridPoint(i,j)), mtm::CellOccupied);
            }
            else{
                ASSERT_ERROR(game.move(selected, GridPoint(i,j)), MoveTooFar);
            }
        }
    }

    return true;

}

bool testAttackGeneric(){

    int rows = 20;
    int cols = 20;

    Game game = Game(rows,cols);

    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(-1,0)), IllegalCell);
    ASSERT_ERROR(game.attack(GridPoint(-1,0), GridPoint(1,0)), IllegalCell);
    ASSERT_ERROR(game.attack(GridPoint(-1,0), GridPoint(-1,0)), IllegalCell);
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(rows,0)), IllegalCell);
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,cols)), IllegalCell);
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(rows,cols)), IllegalCell);
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(rows-1,cols-1)), CellEmpty);

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(rows-1,cols-1)), OutOfRange);
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(1,0)));

    return true;

}

bool testAttackSoldier(){

    int rows = 20;
    int cols = 20;

    Game game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(1,0)));

    game = Game(rows,cols);
    // Note: difference is that range is now 0 instead of 1
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 0, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(1,0)));

    game = Game(rows,cols);
    // Note: difference is that range is now 2 instead of 1
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 2, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(1,0)));

    game = Game(rows,cols);
    // Note: difference is that range is now 4 instead of 1
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 1, 4, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,2), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(2,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(2,1), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 0, 0, 0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(1,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,2)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(2,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(2,1)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(1,1)));

    game = Game(rows,cols);
    int range = 7;
    GridPoint selected(9,9);
    ASSERT_NO_ERROR(game.addCharacter(selected, Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, range, 0)));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint::distance(GridPoint(i,j), selected) <= range && (i==selected.row || j==selected.col)){
                ASSERT_ERROR(game.attack(selected, GridPoint(i, j)), OutOfAmmo);
            }
            else if (GridPoint::distance(GridPoint(i, j), selected) <= range && (i != selected.row || j != selected.col))
            {
                ASSERT_ERROR(game.attack(selected, GridPoint(i, j)), OutOfAmmo);
            }
            else{
                ASSERT_ERROR(game.attack(selected, GridPoint(i, j)), OutOfRange);
            }
        }
    }

    return true;

}

bool testAttackMedic(){
    
    int rows = 20;
    int cols = 20;

    Game game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(MEDIC, POWERLIFTERS, 1, 0, 1, 1)));

    // At time of writing, forum verified the following won't be tested
    // ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)), mtm::IllegalTarget);
    // ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), mtm::IllegalTarget);
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)), mtm::IllegalTarget);

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(MEDIC, POWERLIFTERS, 1, 0, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(MEDIC, CROSSFITTERS, 1, 1, 1, 1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(1,1), GridPoint(0,1)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(1,1)));

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(MEDIC, POWERLIFTERS, 1, 1, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(MEDIC, CROSSFITTERS, 1, 1, 1, 1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(1,1), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), mtm::IllegalTarget);
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(1,1)));

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(MEDIC, POWERLIFTERS, 1, 1, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 1, 0, 1, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(MEDIC, CROSSFITTERS, 1, 1, 1, 1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(1,1), GridPoint(0,1)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,0)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(0,1)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(1,1)));

    game = Game(rows,cols);
    int range = 7;
    GridPoint selected(9,9);
    ASSERT_NO_ERROR(game.addCharacter(selected, Game::makeCharacter(MEDIC, POWERLIFTERS, 1, 0, range, 0)));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (i == selected.row && j == selected.col){
                continue;
            }
            if (GridPoint::distance(GridPoint(i,j), selected) <= range){
                ASSERT_ERROR(game.attack(selected, GridPoint(i,j)), OutOfAmmo);
            }
            else{
                ASSERT_ERROR(game.attack(selected, GridPoint(i,j)), OutOfRange);
            }
        }
    }

    return true;

}

bool testAttackSniper(){

    int rows = 20;
    int cols = 20;

    Game game = Game(rows,cols);
    
    game = Game(rows,cols);
    int range = 7;
    GridPoint selected(9,9);
    ASSERT_NO_ERROR(game.addCharacter(selected, Game::makeCharacter(SNIPER, POWERLIFTERS, 1, 0, range, 0)));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint::distance(GridPoint(i,j), selected) <= range && GridPoint::distance(GridPoint(i,j), selected) >= ceil(double(range)/2)){
                ASSERT_ERROR(game.attack(selected, GridPoint(i,j)), OutOfAmmo);
            }
            else{
                ASSERT_ERROR(game.attack(selected, GridPoint(i,j)), OutOfRange);
            }
        }
    }

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,5), Game::makeCharacter(SNIPER, POWERLIFTERS, 1, 0, 3, 1)));

    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(5,8)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(5,5)));
    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(5,8)), mtm::IllegalTarget);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,8), Game::makeCharacter(SNIPER, POWERLIFTERS, 1, 0, 3, 0)));
    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(5,8)), mtm::IllegalTarget);

    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,2), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(2,5), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(8,5), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,3), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(3,5), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(7,5), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,7), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 0, 0, 0)));

    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(5,2)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(2,5)));
    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(8,5)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(5,5)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(8,5)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(5,3)));
    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(3,5)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(5,5)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(3,5)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(7,5)));
    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(5,7)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(5,5)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(5,7)));
    ASSERT_NO_ERROR(game.attack(GridPoint(5,5), GridPoint(5,7)));
    ASSERT_ERROR(game.attack(GridPoint(5,5), GridPoint(5,7)), OutOfAmmo);

    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(5,2)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(2,5)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(8,5)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(5,3)));
    ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(3,5)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(7,5)));
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(5,7)));

    return true;

}

bool testReload(){

    int rows = 20;
    int cols = 20;

    Game game(rows, cols);

    ASSERT_ERROR(game.reload(GridPoint(-1, 0)), IllegalCell);
    ASSERT_ERROR(game.reload(GridPoint(0, 0)), CellEmpty);

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SOLDIER, CROSSFITTERS, 2, 1, 0, 4)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,0)), OutOfAmmo);

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(MEDIC, CROSSFITTERS, 2, 1, 1, 4)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(MEDIC, POWERLIFTERS, 120, 10, 2, 1)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,0), Game::makeCharacter(MEDIC, CROSSFITTERS, 1, 1, 0, 4)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(1,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_NO_ERROR(game.attack(GridPoint(0,1), GridPoint(1,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,1), GridPoint(1,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,1), GridPoint(1,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,1), GridPoint(1,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,1), GridPoint(1,0)));
    ASSERT_ERROR(game.attack(GridPoint(0,1), GridPoint(1,0)), mtm::IllegalTarget);

    game = Game(rows,cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,0), Game::makeCharacter(SNIPER, CROSSFITTERS, 2, 1, 1, 4)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_NO_ERROR(game.reload(GridPoint(0,0)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_NO_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)));
    ASSERT_ERROR(game.attack(GridPoint(0,0), GridPoint(0,1)), OutOfAmmo);
    ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(0,1)));

    return true;

}

bool testOutput(){

    int rows = 10;
    int cols = 8;
    Game game(rows, cols);
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(2,1), Game::makeCharacter(SOLDIER, CROSSFITTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(4,1), Game::makeCharacter(MEDIC, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(5,2), Game::makeCharacter(MEDIC, CROSSFITTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(3,7), Game::makeCharacter(SNIPER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(9,3), Game::makeCharacter(SNIPER, CROSSFITTERS, 24, 0, 0, 0)));
    
    std::string expected = \
    "*****************\n\
| |S| | | | | | |\n\
| | | | | | | | |\n\
| |s| | | | | | |\n\
| | | | | | | |N|\n\
| |M| | | | | | |\n\
| | |m| | | | | |\n\
| | | | | | | | |\n\
| | | | | | | | |\n\
| | | | | | | | |\n\
| | | |n| | | | |\n\
*****************";
    std::stringstream buffer;
    ASSERT_NO_ERROR(buffer << game);
    
    ASSERT_TEST(expected == buffer.str());

    return true;

}

bool testWinningTeam(){

    int rows = 20;
    int cols = 10;

    Game game(rows, cols);

    ASSERT_TEST(!game.isOver());
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,2), Game::makeCharacter(SOLDIER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,3), Game::makeCharacter(SOLDIER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_TEST(game.isOver());
    ASSERT_TEST(game.isOver(nullptr));

    Team winning = CROSSFITTERS;
    ASSERT_TEST(game.isOver(&winning));
    ASSERT_TEST(winning == POWERLIFTERS);

    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(SOLDIER, CROSSFITTERS, 24, 0, 0, 0)));
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(3,1), Game::makeCharacter(SOLDIER, CROSSFITTERS, 24, 0, 0, 0)));
    ASSERT_TEST(!game.isOver());
    ASSERT_TEST(!game.isOver(&winning));
    ASSERT_TEST(winning == POWERLIFTERS);

    game = Game(rows, cols);

    ASSERT_TEST(!game.isOver());
    ASSERT_NO_ERROR(game.addCharacter(GridPoint(0,1), Game::makeCharacter(SOLDIER, CROSSFITTERS, 24, 0, 0, 0)));
    ASSERT_TEST(game.isOver());
    ASSERT_TEST(game.isOver(nullptr));

    ASSERT_TEST(game.isOver(&winning));
    ASSERT_TEST(winning == CROSSFITTERS);

    ASSERT_NO_ERROR(game.addCharacter(GridPoint(1,1), Game::makeCharacter(SOLDIER, POWERLIFTERS, 24, 0, 0, 0)));
    ASSERT_TEST(!game.isOver());
    ASSERT_TEST(!game.isOver(&winning));
    ASSERT_TEST(winning == CROSSFITTERS);


    return true;

}

bool testGame1(){

	Game g1(8,8);

	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(1,1), Game::makeCharacter(CharacterType::MEDIC, Team::POWERLIFTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(1,4), Game::makeCharacter(CharacterType::SNIPER, Team::POWERLIFTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(6,1), Game::makeCharacter(CharacterType::SOLDIER, Team::CROSSFITTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(6,4), Game::makeCharacter(CharacterType::MEDIC, Team::CROSSFITTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.move(GridPoint(1,1), GridPoint(1,2)));

    ASSERT_ERROR(g1.attack(GridPoint(1,4), GridPoint(1,2)), mtm::IllegalTarget);
    ASSERT_ERROR(g1.attack(GridPoint(1,4), GridPoint(6,1)), OutOfRange);
    ASSERT_NO_ERROR(g1.move(GridPoint(1,4), GridPoint(3,2)));
    ASSERT_ERROR(g1.attack(GridPoint(1,4), GridPoint(6,4)), CellEmpty);
	ASSERT_NO_ERROR(g1.attack(GridPoint(3,2), GridPoint(6,1)));
    ASSERT_NO_ERROR(g1.move(GridPoint(6,1), GridPoint(4,2)));
    ASSERT_ERROR(g1.attack(GridPoint(3,2), GridPoint(4,2)), OutOfRange);
    ASSERT_NO_ERROR(g1.move(GridPoint(4,2), GridPoint(6,2)));
    ASSERT_NO_ERROR(g1.attack(GridPoint(3,2), GridPoint(6,2)));
	ASSERT_ERROR(g1.move(GridPoint(6,2), GridPoint(6,1)), CellEmpty);
    ASSERT_NO_ERROR(g1.move(GridPoint(3,2), GridPoint(3,4)));
    ASSERT_ERROR(g1.attack(GridPoint(3,4), GridPoint(6,4)), OutOfAmmo);
    ASSERT_ERROR(g1.attack(GridPoint(6,4), GridPoint(6,4)), mtm::IllegalTarget);

	Team winning_team = Team::CROSSFITTERS;
	
	ASSERT_TEST(!g1.isOver(&winning_team));
    ASSERT_TEST(winning_team == CROSSFITTERS);

	ASSERT_NO_ERROR(g1.reload(GridPoint(3,4)));
	ASSERT_NO_ERROR(g1.attack(GridPoint(3,4), GridPoint(6,4)));
	ASSERT_ERROR(g1.move(GridPoint(6,4), GridPoint(6,1)), CellEmpty);
    ASSERT_TEST(g1.isOver(&winning_team));
    ASSERT_TEST(winning_team == POWERLIFTERS);

    return true;

}

bool testGame2(){

	Game g1(5,10);
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(3,0), Game::makeCharacter(CharacterType::SOLDIER, Team::POWERLIFTERS, 20, 0, 3, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(2,6), Game::makeCharacter(CharacterType::SNIPER, Team::CROSSFITTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(3,8), Game::makeCharacter(CharacterType::SNIPER, Team::CROSSFITTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(3,6), Game::makeCharacter(CharacterType::MEDIC, Team::CROSSFITTERS, 10, 2, 4, 5)));
	ASSERT_NO_ERROR(g1.addCharacter(GridPoint(4,6), Game::makeCharacter(CharacterType::MEDIC, Team::CROSSFITTERS, 10, 2, 4, 5)));

    ASSERT_ERROR(g1.attack(GridPoint(3,0), GridPoint(3,6)), OutOfRange);
	ASSERT_ERROR(g1.move(GridPoint(3,0), GridPoint(3,4)), MoveTooFar);
	
	Game g2 = g1;
	
	ASSERT_NO_ERROR(g1.move(GridPoint(3,0), GridPoint(3,3)));
	ASSERT_ERROR(g1.attack(GridPoint(3,3), GridPoint(3,6)), OutOfAmmo);
    ASSERT_ERROR(g1.reload(GridPoint(3,2)), CellEmpty);
    ASSERT_ERROR(g1.reload(GridPoint(3,-3)), IllegalCell);
    ASSERT_ERROR(g1.reload(GridPoint(3,13)), IllegalCell);
	ASSERT_NO_ERROR(g1.reload(GridPoint(3,3)));
    ASSERT_NO_ERROR(g1.attack(GridPoint(3,3), GridPoint(3,6)));
    ASSERT_NO_ERROR(g1.attack(GridPoint(3,3), GridPoint(3,6)));
    ASSERT_NO_ERROR(g1.attack(GridPoint(3,3), GridPoint(3,6)));
    ASSERT_NO_ERROR(g1.attack(GridPoint(4,6), GridPoint(2,6)));
    ASSERT_ERROR(g1.attack(GridPoint(4,6), GridPoint(4,6)), IllegalTarget);
    ASSERT_ERROR(g1.attack(GridPoint(3,3), GridPoint(3,6)), OutOfAmmo);
    ASSERT_NO_ERROR(g1.reload(GridPoint(3,3)));
    ASSERT_NO_ERROR(g1.reload(GridPoint(3,3)));
    ASSERT_NO_ERROR(g1.attack(GridPoint(3,3), GridPoint(3,6)));

    return true;

}

bool testGame3(){

    int rows = 80;
    int cols = 80;
    Game game(rows, cols);
    GridPoint selected(15, 20);
    int range = 20;
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint(i,j) == selected){
                ASSERT_NO_ERROR(game.addCharacter(GridPoint(i,j), Game::makeCharacter(SOLDIER, CROSSFITTERS, 1, 2, range, 2)));
            }
            else{
                ASSERT_NO_ERROR(game.addCharacter(GridPoint(i,j), Game::makeCharacter(SNIPER, POWERLIFTERS, 2, 0, 0, 0)));
            }
        }
    }
    GridPoint target(35, 20);
    ASSERT_NO_ERROR(game.attack(selected, target));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint(i,j) == selected){
                ASSERT_TEST(checkGameContainsPlayerAt(game, selected));
            }
            else if (GridPoint(i,j) == target){
                ASSERT_TEST(!checkGameContainsPlayerAt(game, target));
            }
            else if (GridPoint::distance(GridPoint(i,j), target) <= ceil(double(range)/3)){
                ASSERT_TEST(checkGameContainsPlayerAt(game, GridPoint(i,j)));
            }
        }
    }
    ASSERT_NO_ERROR(game.attack(selected, target));
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (GridPoint(i,j) == selected){
                ASSERT_TEST(checkGameContainsPlayerAt(game, selected));
            }
            else if (GridPoint(i,j) == target){
                ASSERT_TEST(!checkGameContainsPlayerAt(game, target));
            }
            else if (GridPoint::distance(GridPoint(i,j), target) <= ceil(double(range)/3)){
                ASSERT_TEST(!checkGameContainsPlayerAt(game, GridPoint(i,j)));
            }
        }
    }

    ASSERT_ERROR(game.attack(selected, target), OutOfAmmo);

    return true;

}

bool run_test(std::function<bool()> test, std::string test_name){
    if(!test()){
        cout<<test_name<<" - FAILED."<<endl;
        return false;
    }
    cout<<test_name<<" - PASSED."<<endl;
    return true;
}

int main(){

    std::map<std::string, std::function<bool()>> tests;

    ADD_TEST(testErrorNames);
    ADD_TEST(testConstructor);
    ADD_TEST(testCopyConstructor);
    ADD_TEST(testAssignemntOperator);
    ADD_TEST(testAddCharacter);
    ADD_TEST(testMakeCharacter);
    ADD_TEST(testMove);
    ADD_TEST(testAttackGeneric);
    ADD_TEST(testAttackSoldier);
    ADD_TEST(testAttackMedic);
    ADD_TEST(testAttackSniper);
    ADD_TEST(testReload);
    ADD_TEST(testOutput);
    ADD_TEST(testWinningTeam);
    ADD_TEST(testGame1);
    ADD_TEST(testGame2);
    ADD_TEST(testGame3);

    int passed = 0;
    for (std::pair<std::string, std::function<bool()>> element : tests)
    {
        passed += run_test(element.second, "Running " + element.first);
    }

    cout<<"Passed "<<passed<<" out of "<<tests.size()<<" tests."<<endl;

    return 0;
}
