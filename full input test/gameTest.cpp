#include <iostream>

#include <cassert>

#include "Exceptions.h"
#include "Game.h"

using namespace mtm;

void example1();
void example2();

int main() {
    example1();
    example2();
}

void example1() {
    std::cout << "------example 1------" << std::endl;
    Game g1(8,8);
    g1.addCharacter(GridPoint(1,1), Game::makeCharacter(CharacterType::MEDIC, Team::POWERLIFTERS, 10, 2, 4, 5));
    g1.addCharacter(GridPoint(1,4), Game::makeCharacter(CharacterType::SNIPER, Team::POWERLIFTERS, 10, 2, 4, 5));
    g1.addCharacter(GridPoint(6,1), Game::makeCharacter(CharacterType::SOLDIER, Team::CROSSFITTERS, 10, 2, 4, 5));
    g1.addCharacter(GridPoint(6,4), Game::makeCharacter(CharacterType::MEDIC, Team::CROSSFITTERS, 10, 2, 4, 5));
    std::cout << g1 << std::endl;
    g1.move(GridPoint(1,1), GridPoint(1,2));
    std::cout << g1 << std::endl;


    try {
        g1.attack(GridPoint(1,4), GridPoint(1,2)); // can't attak ally
    } catch (mtm::IllegalTarget& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        g1.attack(GridPoint(1,4), GridPoint(6,1)); // should not work - not in range
    } catch (mtm::OutOfRange& e) {
        std::cout << e.what() << std::endl;
    }

    g1.move(GridPoint(1,4), GridPoint(3,2));
    std::cout << g1 << std::endl;
    try {
        g1.attack(GridPoint(1,4), GridPoint(6,4)); // character moved away
    } catch (mtm::CellEmpty& e) {
        std::cout << e.what() << std::endl;
    }

    g1.attack(GridPoint(3,2), GridPoint(6,1)); // now it can hit

    g1.move(GridPoint(6,1), GridPoint(4,2));
    std::cout << g1 << std::endl;

    try {
        g1.attack(GridPoint(3,2), GridPoint(4,2)); // sniper can't attack close targets
    } catch (mtm::OutOfRange& e) {
        std::cout << e.what() << std::endl;
    }

    g1.move(GridPoint(4,2), GridPoint(6,2));
    std::cout << g1 << std::endl;

    g1.attack(GridPoint(3,2), GridPoint(6,2)); // now it can hit
    std::cout << g1 << std::endl;

    try {
        g1.move(GridPoint(6,2), GridPoint(6,1)); // soldier was killed and removed
    } catch (mtm::CellEmpty& e) {
        std::cout << e.what() << std::endl;
    }

    g1.move(GridPoint(3,2), GridPoint(3,4));

    try {
        g1.attack(GridPoint(3,4), GridPoint(6,4)); // sniper out of ammo
    } catch (mtm::OutOfAmmo& e) {
        std::cout << e.what() << std::endl;
    }


    try {
        g1.attack(GridPoint(6,4), GridPoint(6,4)); // medic can't heal itself
    } catch (mtm::IllegalTarget& e) {
        std::cout << e.what() << std::endl;
    }

    mtm::Team winning_team = mtm::Team::CROSSFITTERS;
    assert(g1.isOver(&winning_team) == false);
    std::cout << "isOver: " <<  g1.isOver(&winning_team) << std::endl;
    std::cout << "is powerlifters the winning team?: " <<  (winning_team == mtm::Team::POWERLIFTERS) << std::endl;

    g1.reload(GridPoint(3,4));
    g1.attack(GridPoint(3,4), GridPoint(6,4)); // now can shoot

    std::cout << g1 << std::endl;

    try {
        g1.move(GridPoint(6,4), GridPoint(6,1)); // medic was killed and removed
        // the kill was possible because of the sniper double power third attack
    } catch (mtm::CellEmpty& e) {
        std::cout << e.what() << std::endl;
    }

    assert(g1.isOver(&winning_team) == true);
    assert(winning_team == mtm::Team::POWERLIFTERS);
    std::cout << "isOver: " <<  g1.isOver(&winning_team) << std::endl;
    std::cout << "is powerlifters the winning team?: " <<  (winning_team == mtm::Team::POWERLIFTERS) << std::endl;
}

void example2() {
    std::cout << "------example 2------" << std::endl;
    Game g1(5,10);
    g1.addCharacter(GridPoint(3,0), Game::makeCharacter(CharacterType::SOLDIER, Team::POWERLIFTERS, 20, 0, 3, 5));
    g1.addCharacter(GridPoint(2,6), Game::makeCharacter(CharacterType::SNIPER, Team::CROSSFITTERS, 10, 2, 4, 5));
    g1.addCharacter(GridPoint(3,8), Game::makeCharacter(CharacterType::SNIPER, Team::CROSSFITTERS, 10, 2, 4, 5));
    g1.addCharacter(GridPoint(3,6), Game::makeCharacter(CharacterType::MEDIC, Team::CROSSFITTERS, 10, 2, 4, 5));
    g1.addCharacter(GridPoint(4,6), Game::makeCharacter(CharacterType::MEDIC, Team::CROSSFITTERS, 10, 2, 4, 5));
    std::cout << g1 << std::endl;

    try {
        g1.attack(GridPoint(3,0), GridPoint(3,6));
    } catch (mtm::OutOfRange& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        g1.move(GridPoint(3,0), GridPoint(3,4)); // soldier only moves 3 at a time
    } catch (mtm::MoveTooFar& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << g1 << std::endl; // has not changed

    Game g2 = g1; // copy constructor - games should be independent

    g1.move(GridPoint(3,0), GridPoint(3,3));

    std::cout << g1 << std::endl; // changed
    std::cout << g2 << std::endl; // has not changed

    try {
        g1.attack(GridPoint(3,3), GridPoint(3,6)); // soldier has 0 ammo
    } catch (mtm::OutOfAmmo& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        g1.reload(GridPoint(3,2));
    } catch (mtm::CellEmpty& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        g1.reload(GridPoint(3,-3));
    } catch (mtm::IllegalCell& e) {
        std::cout << e.what() << std::endl;
    }
    try {
        g1.reload(GridPoint(3,13));
    } catch (mtm::IllegalCell& e) {
        std::cout << e.what() << std::endl;
    }

    g1.reload(GridPoint(3,3)); // now soldier has ammo

    g1.attack(GridPoint(3,3), GridPoint(3,6)); // damages 2 medics and 1 sniper in range
    std::cout << g1 << std::endl; // everyone still alive

    g1.attack(GridPoint(3,3), GridPoint(3,6)); // one medic dies (took 5+5 damage instead of 3+3 like the others)
    std::cout << g1 << std::endl;

    g1.attack(GridPoint(3,3), GridPoint(3,6));
    std::cout << g1 << std::endl;

    g1.attack(GridPoint(4,6), GridPoint(2,6)); // medic heals sniper
    try {
        g1.attack(GridPoint(4,6), GridPoint(4,6)); // medic can't heal self
    } catch (mtm::IllegalTarget& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        g1.attack(GridPoint(3,3), GridPoint(3,6)); // soldier out of ammo
    } catch (mtm::OutOfAmmo& e) {
        std::cout << e.what() << std::endl;
    }

    g1.reload(GridPoint(3,3)); // now soldier has ammo
    g1.reload(GridPoint(3,3)); // can reload twice - no problem (more ammo)

    g1.attack(GridPoint(3,3), GridPoint(3,6)); // medic dead, sniper not dead
    std::cout << g1 << std::endl;
}