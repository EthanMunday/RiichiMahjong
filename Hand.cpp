#include "Hand.h"

Hand::Hand()
{
	// Every hand starts closed
	handOpen = false;
}

void Hand::DiscardTile(short discard)
{
	// Sorts the hand to ensure the right tile is discarded
	SortHand();
	// Creates a replica tile to add to the discard pile
	Tile tempTile = tiles[discard];
	// Erases the tile at the passed index
	tiles.erase(tiles.begin()+discard);
	// Adds the replica tile to the discard pile
	discardPile.push_back(tempTile);
}

void Hand::SetDirection(string seat)
{
	// Sets the wind direction
	direction = seat;
}

void Hand::SortHand()
{
	// Sorts the hand using some fancy algorithm
	// Uses operator< from the Tile class
	sort(tiles.begin(), tiles.end());
}