#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Hand.h"
using namespace std;

class Player
{
public:
	// The hand of the player
	Hand hand;
	// The points a player has
	int points;
	// Determines wether a player is in Richii
	bool inRichi;
	// Determines wether a player is in Tenpai
	bool inTenpai;
	// Determines wether a hand is valid
	bool handValid;
	// Determines wether a hand is both valid and has value
	bool handTrue;
	
	// Determines wether a hand is made up of 4 runs/triples and 1 pair
	// Except in Seven Pairs and Thirteen Orphans hands
	void validateHand();
	// Overloaded to continue a searching/calculation algorithm
	// It's really long and dumb
	void validateHand(vector <Tile> passedTiles, vector <Tile> passedOpenZone);
	// Checks if the players hand is in Tenpai
	void checkTenpai(vector <Tile> tenpaiTiles);
	// Used in validateHand to act as a seperate handValid
	bool tempValid;

	// Used after validateHand finds a valid hand to check that hands value
	void checkHandValue(vector <Tile> validOpenZone);
	// The value of the winning hand
	int han;
	// The list of hand types that a winning hands achieves 
	vector <string> handTypes;

	// Checks 2 tiles for a pair
	bool pair(Tile x, Tile y);
	// Checks 3 tiles for a triple
	bool triple(Tile x, Tile y, Tile z);
	// Checks 3 tiles for a run
	bool run(Tile x, Tile y, Tile z);
	// Checks if a hand has a Seven Pairs hand
	bool sevenPairs(vector <Tile> check);
	// Checks if a hand has a Thirteen Orphans hand
	bool thirteenOrphans(vector <Tile> check);
	// Checks if a hand has a Nine Needs hand
	bool nineNeeds(vector <Tile> check);
	// Checks if a hand can call Pon on a tile
	bool pon(Tile x);
	// Activates the call for Pon
	void sendPon(Tile x);
	// Checks if a hand can call Chii on a tile
	bool chi(Tile x);
	// Activates the call for Chii
	void sendChi(Tile x);

	// Constructor
	Player();
	// Destructor
	~Player();
};