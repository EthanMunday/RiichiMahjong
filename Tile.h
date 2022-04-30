#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <algorithm>
using namespace std;

class Tile
{
private:
	// Used to change console colours
	HANDLE  hConsole;
public:
	// The order in which a tile is sorted
	short priorityOrder;
	// The console colour of the tile
	short colour;
	// The value of a tile
	// 1-9 for red,blue,green, 0 for honour tiles
	short value;
	// The type of a tile
	// red,green,blue,wind,dragon
	string type;
	// The displayed text of a tile
	string display;

	// Used to sort the hand into the correct order
	bool operator< (const Tile& other) const;
	// Displays the tile
	void DisplayTile();
	// Creates a red, green or blue tile
	Tile CreateTile(string colour, short value);
	// Creates a wind or dragon tile
	Tile CreateTile(string honour, string type);

	// Constructor
	Tile();
};