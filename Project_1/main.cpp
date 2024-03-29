#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <ctime>
#include "globals.h"
#include "Zombie.h"
#include "Player.h"
#include "Arena.h"
#include "Game.h"
#include "History.h"
using namespace std;

int main()
{
      // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));

      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(4, 4, 3);

      // Play the game
    g.play();
}
