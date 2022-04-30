#include "Deck.h"

void Deck::createDeck()
{
	// Creates a temporary tile
	// Very inefficient, definitely an easier way to create tiles
	Tile tempTile;
	// Repeat 4 times
	for (short a = 0; a < 4; a++)
	{
		// Add red,blue and green tiles from 1-9
		for (short b = 1; b < 10; b++)
		{
			tiles.push_back(tempTile.CreateTile("red", b));
			tiles.push_back(tempTile.CreateTile("green", b));
			tiles.push_back(tempTile.CreateTile("blue", b));
		}
		// Adds one of each dragon and wind tile
		tiles.push_back(tempTile.CreateTile("dragon", "red"));
		tiles.push_back(tempTile.CreateTile("dragon", "green"));
		tiles.push_back(tempTile.CreateTile("dragon", "white"));
		tiles.push_back(tempTile.CreateTile("wind", "north"));
		tiles.push_back(tempTile.CreateTile("wind", "east"));
		tiles.push_back(tempTile.CreateTile("wind", "south"));
		tiles.push_back(tempTile.CreateTile("wind", "west"));
	}
}

void Deck::reduceDeckSize()
{
	// Reduces deckRemaining by 1
	deckRemaining -= 1;
}

void Deck::setDeckSize(short size)
{
	// Sets deckRemaining to passed short
	deckRemaining = size;
}

short Deck::deckSize()
{
	// Gets deckRemaining
	return deckRemaining;
}

Deck::Deck()
{
	deckRemaining = 136;
}