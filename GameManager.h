#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "Player.h"
#include "Deck.h"
using namespace std;

class GameManager
{
private:
	// Tracks the current player looking at the screen
	short currentPlayer;
protected:
	// Tracks the deck
	Deck deck;
	// Tracks the players
	Player player[4];
	// Tracks the last discarded tile for call use
	Tile discardTile;

	// Used in code to trigger a select menu chosen by the passed variables
	short selectMenu(string variant, short selectPlayer);
	// Used in the above function to pass menu options into an actual output
	short selectMenu(vector <string> variant);

	// Get/Set for the currentPlayer
	short getCurrentPlayer();
	short setCurrentPlayer(short newNumber);

	// Draws a tile to a given player
	Tile giveTile(short drawPlayer);
	// Draws 13 tiles to each players hand
	void initialDraw();

	// Outputs a menu to tell a player to take control. Also changes currentPlayer.
	void playerControl(short controlledPlayer);
	// Outputs the current state of the game.
	void displayGame();
	// Outputs the points of each player
	void displayPoints();
	// Outputs the final end screen
	void endScreen();

	// Starts 1 round of the game
	void startGame(short eastPlayer);
	// Loops turns until the game is over
	void gameLoop();

	// Used to check for calls.
	bool chiCheck();
	// Used to call Tsumo
	void tsumo(short winPlayer);
	// Used to call Ron
	void ron(short winPlayer,short deadPlayer);
	// Used to check for Ron validity
	bool ronCheck(short turnPlayer);

	// Advanced std::cin barrier to get a continue input
	void getchLoop();

public:
	// Starts the full game
	void startGame();
	// Constructor
	GameManager();
};