#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "Tile.h"
using namespace std;

class Deck
{
private:
	// The amount of tiles remaining in the deck
	short deckRemaining;

public:
	// The tiles in the deck
	vector <Tile> tiles;

	// Returns the size of the deck by 1
	void reduceDeckSize();
	// Get/Set for deckRemaining
	void setDeckSize(short size);
	short deckSize();

	// Adds a whole deck to the deck
	void createDeck();

	// Constructor
	Deck();
};