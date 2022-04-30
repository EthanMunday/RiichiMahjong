#include "Tile.h"

Tile::Tile()
{
	// Starts off as a blank tile with no value
	colour = NULL;
	priorityOrder = NULL;
	value = 0;
	type = "";
	display = "";
	// Gets the console to change console colours or something
	// Not to sure what this does but it works
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Tile::DisplayTile()
{
	// Sets the console colour to the colour of the tile using the weird hConsole thing
	SetConsoleTextAttribute(hConsole, colour);
	// Displays the tile
	cout << display;
	// Sets the colour back
	SetConsoleTextAttribute(hConsole, 15);
}

bool Tile::operator<(const Tile& other) const
{
	// This is the cool sorting algorithm for the tiles
	// Basically returning false swaps the tiles, or is it true? Either way you get the idea
	// If the priority of the tile is the same
	if (priorityOrder == other.priorityOrder)
	{
		// Returns true if the other tile has a larger value
		return (value < other.value);
	}
	// Otherwise, returns true if the other tile has a larger priority value
	return (priorityOrder < other.priorityOrder);
}

Tile Tile::CreateTile(string colour, short value)
{
	// As I mentioned in the Deck class, needing a tile to create a tile is a design flaw
	// Since I need to spawn a tile in to create one
	// This function works for red,blue and green tiles
	// Creates a new Tile
	Tile testTile;
	// Sets the type to the passed string
	testTile.type = colour;
	// Sets the value to the clamp of whatever value is passed
	testTile.value = clamp(int(value), 1, 9);
	// Sets the display to the string variant of the value
	testTile.display = to_string(value);
	// Changes the colour depending on the passed string
	if (colour == "green")
	{
		testTile.colour = 32;
		testTile.priorityOrder = 2;
	}
	if (colour == "blue")
	{
		testTile.colour = 48;
		testTile.priorityOrder = 1;
	}
	if (colour == "red")
	{
		testTile.colour = 64;
		testTile.priorityOrder = 0;
	}
	// Returns the modified tile
	return testTile;
}

Tile Tile::CreateTile(string honour, string type)
{
	// This function works for honour tiles (wind and dragon)
	// Creates a new tile
	Tile testTile;
	// Sets the type to the first passed string
	testTile.type = honour;
	// If the tile is a dragon
	if (honour == "dragon")
	{
		// Set the display to 'D' and the colour to either white,green or red
		testTile.display = "D";
		if (type == "white")
		{
			testTile.colour = 240;
			testTile.priorityOrder = 9;
		}
		if (type == "green")
		{
			testTile.colour = 32;
			testTile.priorityOrder = 8;
		}
		if (type == "red")
		{
			testTile.colour = 64;
			testTile.priorityOrder = 7;
		}
	}
	// If the tile is a wind
	if (honour == "wind")
	{
		// Set the colour to light gray and the display to either 'N','E','S','W'
		testTile.colour = 112;
		if (type == "north")
		{
			testTile.display = "N";
			testTile.priorityOrder = 3;
		}
		if (type == "east")
		{
			testTile.display = "E";
			testTile.priorityOrder = 4;
		}
		if (type == "south")
		{
			testTile.display = "S";
			testTile.priorityOrder = 5;
		}
		if (type == "west")
		{
			testTile.display = "W";
			testTile.priorityOrder = 6;
		}
	}

	// Returns the modified tile
	return testTile;


	/*
	Also forgot to mention, priority order is decided based on wether your tile is
	red,green,blue, red,white or green dragon  or N/E/S/W wind
	This was added later into development to help with the sorting function
	It was also used when comparing tiles for triples and runs later on
	*/
}