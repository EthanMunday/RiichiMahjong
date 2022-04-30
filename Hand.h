#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "Tile.h"
using namespace std;

class Hand
{
public:
	// The hidden tiles of a hand
	vector<Tile> tiles;
	// The open tiles of a hand
	vector<Tile> openZone;
	// The tiles that have been discarded by a hand
	vector<Tile> discardPile;
	// The wind direction of the hand (Majong things)
	string direction;
	// Determines if the hand has any open tiles
	bool handOpen;

	// Discards a tile from the hand
	void DiscardTile(short discard);
	// Sets the wind direction of the hand
	void SetDirection(string seat);

	// Sorts the hand
	// Hands are sorted reds,blues,greens,winds,dragons in value order.
	void SortHand();

	// Constructor
	Hand();
};