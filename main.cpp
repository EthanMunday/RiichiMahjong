#include <iostream>
#include <windows.h>
#include "GameManager.h"
using namespace std;

int main()
{	// Game Manager Class being used to handle all game logic from within an object.
	GameManager* game1 = new GameManager;
	// Starts the game
	game1->startGame();
	// Deconstructor but for the Game Manager
	delete game1;
	game1 = nullptr;
}