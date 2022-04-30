#include "Player.h"

Player::Player()
{
	// Starting points for majong vary but I choose 20000
	points = 20000;
	han = 0;
	inRichi = false;
	inTenpai = false;
	handValid = false;
	handTrue = false;
	tempValid = false;
}

Player::~Player()
{
}

void Player::validateHand()
{
	/*
	HOO BOY. Buckle up.
	Gonna need some explanation here.
	So each valid hand has 14 tiles. 4 runs/triples and 1 pair.
	The only exceptions to this are the hands 13 Orphans and 7 pairs.
	So for every triple, run or pair, you need to calculate every possible triple, run or pair option to check if a hand is valid.
	On top of checking for 7 pairs and 13 Orphans.
	The way I tackled this was for each pair/triple/run found, duplicate that path and repeat the pair/triple/run checks.
	This was the hardest logic and programming puzzle of the game.
	*/

	// Sets tempValid and inTenpai to false to avoid interference with process
	tempValid = false;
	inTenpai = false;
	// Sorts the players hand before checking.
	hand.SortHand();
	// Duplicate the current players hand and open zone
	vector <Tile> testTiles = hand.tiles;
	vector <Tile> testOpenZone = hand.openZone;
	
	// Checks if Thirteen Orphans hand is achieved
	if (thirteenOrphans(testTiles))
	{
		// If so, skip checking process and calculate hand value
		handValid = true;
		tempValid = true;
		checkHandValue(testTiles);
		return;
	}

	// Same as above but for Seven Pairs
	if (sevenPairs(testTiles))	
	{
		if (short(testTiles.size()) == 14)
		{
			handValid = true;
			tempValid = true;
			checkHandValue(testTiles);
			return;
		}
		
	}

	//The first step is to search the duped hand for a pair since the hand cannot be valid otherwise
	for (short x = 0;  x < short(testTiles.size()) - 1; x++)
	{
		if (tempValid == false)
		{
			// Since the hand is sorted, a pair will appear next to each other in the index
			short y = x + 1;
			// If there is a pair
			if (pair(testTiles[x], testTiles[y]))		
			{
				// Duplicate the original hand and starting zone to avoid it interfering with the first dupe
				vector <Tile> firstTestTiles = testTiles;
				vector <Tile> firstTestOpenZone = testOpenZone;
				// Duplicate the pair tiles
				Tile moveX = testTiles[x];
				Tile moveY = testTiles[y];
				// Erase both tile in the list in reverse order to ensure the right tiles are erased
				firstTestTiles.erase(firstTestTiles.begin() + y);
				firstTestTiles.erase(firstTestTiles.begin() + x);
				// Move both tiles into the open zone
				// The open zone is used here as a second hand where valid pairs/triples/runs are sent
				// This also means that tiles sent to the open zone before hand are not searched through
				// If the hand ever becomes empty and the open zone full, that means all tiles fit into a valid format
				firstTestOpenZone.push_back(moveX);
				firstTestOpenZone.push_back(moveY);
				// Starts the validation loop with the current duped hands
				validateHand(firstTestTiles, firstTestOpenZone);		
			}
		}
	}

	
	// If the hand is ever identified as valid
	if (tempValid == true)
	{
		// Set it as such
		handValid = true;
		return;
	}
	// If not
	else
	{
		// Don't do that
		handValid = false;
	}
}

void Player::validateHand(vector <Tile> passedTiles, vector <Tile> passedOpenZone)
{
	// Duplicates the hand and open zone for checking and moving
	// Now that I think about it, this part is useless as I could've used the passed variables later
	vector <Tile> testTiles = passedTiles;
	vector <Tile> testOpenZone = passedOpenZone;

	// Checks for runs
	for (short x = 0; x < short(testTiles.size()) - 2; x++)
	{
		// Runs gave me a lot of problems so I ended up searching every possible combination
		// Very slow for it's purpose
		for (short y = x + 1; y < short(testTiles.size()) - 1; y++)
		{
			for (short z = y + 1; z < short(testTiles.size()); z++)
			{
				// If a run is found
				if (run(testTiles[x], testTiles[y], testTiles[z]))
				{
					// Breaks function if hand is already valid
					if (tempValid == true)
					{
						return;
					}
					// Duplicates hand and open zone to be changed
					vector <Tile> newTestTiles = testTiles;
					vector <Tile> newTestOpenZone = testOpenZone;
					// Duplicates run found
					Tile moveX = testTiles[x];
					Tile moveY = testTiles[y];
					Tile moveZ = testTiles[z];
					// Erases run in reverse order to ensure the right tiles are erased
					newTestTiles.erase(newTestTiles.begin() + z);
					newTestTiles.erase(newTestTiles.begin() + y);
					newTestTiles.erase(newTestTiles.begin() + x);
					// Adds the run to the open zone
					newTestOpenZone.push_back(moveX);
					newTestOpenZone.push_back(moveY);
					newTestOpenZone.push_back(moveZ);
					// Repeats loop on changed hand/open zone
					validateHand(newTestTiles, newTestOpenZone);
				}
			}
		}
	}

	for (short x = 0; x < short(testTiles.size()) - 2; x++)	// Checks for triples
	{
		// Breaks function if hand is already valid
		if (tempValid == true)
		{
			return;
		}

		// Triples are always found next to each other in index
		short y = x + 1;
		short z = y + 1;
		if (triple(testTiles[x], testTiles[y], testTiles[z]))
		{
			// Duplicates hand and open zone to be changed
			vector <Tile> newTestTiles = testTiles;
			vector <Tile> newTestOpenZone = testOpenZone;
			// Duplicates triple found
			Tile moveX = testTiles[x];
			Tile moveY = testTiles[y];
			Tile moveZ = testTiles[z];
			// Erases triple in reverse order to ensure the right tiles are erased
			newTestTiles.erase(newTestTiles.begin() + z);
			newTestTiles.erase(newTestTiles.begin() + y);
			newTestTiles.erase(newTestTiles.begin() + x);
			// Adds the triple to the open zone
			newTestOpenZone.push_back(moveX);
			newTestOpenZone.push_back(moveY);
			newTestOpenZone.push_back(moveZ);
			// Repeats loop on changed hand/open zone
			validateHand(newTestTiles, newTestOpenZone);
		}
	}

	// Breaks function if hand is already valid
	if (tempValid == true)
	{
		return;
	}

	// If the hand has been completely moved to the open zone
	if (testTiles.size() == 0)
	{
		// The hand is valid and points are calculated
		tempValid = true;
		checkHandValue(testOpenZone);
		// Starts to break out
		return;
	}

	// If hand could maybe be valid with 1 tile
	if (testTiles.size() < 3)
	{
		// Check if it's possible and set inTenpai acordingly 
		checkTenpai(testTiles);
		// Breaks out once
		return;
	}
}

void Player::checkHandValue(vector<Tile> validOpenZone)
{
	/*
	For this next part, basically every hand type has a value
	This functions checks the hand for these setups and adds value if they achieve them
	Just Google the hands if you need an explanation, there are too many for me to write up about them
	*/

	// Sets hand value to 0
	han = 0;
	// Check if hand has 14 tiles
	if (validOpenZone.size() != 14)
	{
		return;
	}
	
	// Check if the player declared Richi
	if (inRichi)
	{
		han++;
		handTypes.push_back("Richi");
	}

	// Check for Dragon Tiles hand
	for (short x = 2; x < short(validOpenZone.size()) - 2; x+= 3)
	{
		short y = x + 1;
		short z = y + 1;
		if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]))
		{
			if (validOpenZone[x].type == "dragon" && validOpenZone[y].type == "dragon" && validOpenZone[x].type == "dragon")
			{
				han++;
				handTypes.push_back("Dragon Tiles");
				break;
			}
		}
	}

	// Check for All Simples hand
	short allSimpleCount = 0;
	for (short x = 0; x < short(validOpenZone.size()); x++)
	{
		if (validOpenZone[x].value != 1 && validOpenZone[x].value != 9 && validOpenZone[x].value != 0)
		{
			allSimpleCount++;
		}
		if (allSimpleCount == 14)
		{
			han++;
			handTypes.push_back("All Simples");
			break;
		}
	}

	// Check for All Runs hand
	short allRunsCount = 0;
	for (short x = 2; x < short(validOpenZone.size()) - 2; x += 3)
	{
		short y = x + 1;
		short z = y + 1;
		if (run(validOpenZone[x], validOpenZone[y], validOpenZone[z]))
		{
			allRunsCount++;
		}
		if (allRunsCount == 4)
		{
			han++;
			handTypes.push_back("All Runs");
			break;
		}
	}

	// Check for Closed Sumo hand
	if (hand.handOpen == false)
	{
		han++;
		handTypes.push_back("Closed Tsumo");
	}
	
	// Check for Twin Runs and Double Twin Runs hand
	bool closeTwinRunLoop = false;
	short twinRunCount = 0;
	for (short x = 2; x < short(validOpenZone.size()) - 5; x += 3)
	{
		if (closeTwinRunLoop)
		{
			break;
		}
		for (short a = x + 3; a < short(validOpenZone.size()) - 2; a += 3)
		{
			short y = x + 1;
			short z = y + 1;
			short b = a + 1;
			short c = b + 1;
			if (run(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && run(validOpenZone[a], validOpenZone[b], validOpenZone[c]))
			{
				if (pair(validOpenZone[x], validOpenZone[a]) && pair(validOpenZone[y], validOpenZone[b]) && pair(validOpenZone[z], validOpenZone[c]))
				{
					twinRunCount++;
				}
			}
		}
		
	}
	if (twinRunCount == 1)
	{
		han++;
		handTypes.push_back("Twin Runs");
		closeTwinRunLoop = true;
	}
	if (twinRunCount == 2)
	{
		han += 8;
		handTypes.push_back("Double Twin Runs");
		closeTwinRunLoop = true;
	}

	// Check for Auspicious Winds and Perfect Winds hand
	for (short x = 2; x < short(validOpenZone.size()) - 2; x += 3)
	{
		short y = x + 1;
		short z = y + 1;
		if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && validOpenZone[x].type == "wind")
		{
			if (validOpenZone[x].display == hand.direction && validOpenZone[x].display == "E")
			{
				han+=2;
				handTypes.push_back("Perfect Winds");
				break;
			}

			if (validOpenZone[x].display == hand.direction || validOpenZone[x].display == "E")
			{
				han++;
				handTypes.push_back("Auspicious Winds");
				break;
			}
		}
	}

	// Check for All Triples hand
	short allTriplesCount = 0;
	for (short x = 2; x < short(validOpenZone.size()) - 2; x += 3)
	{
		short y = x + 1;
		short z = y + 1;
		if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]))
		{
			allTriplesCount++;
		}
		if (allTriplesCount == 4)
		{
			han+=2;
			handTypes.push_back("All Triples");
			break;
		}
	}

	// Check for Tricolour Run hand
	bool closeTricolourRunLoop = false;
	for (short x = 2; x < short(validOpenZone.size()) - 8; x += 3)
	{
		if (closeTricolourRunLoop)
		{
			break;
		}
		for (short a = x + 3; a < short(validOpenZone.size()) - 5; a += 3)
		{
			if (closeTricolourRunLoop)
			{
				break;
			}
			for (short d = a + 3; d < short(validOpenZone.size()) - 2; d += 3)
			{
				short y = x + 1;
				short z = y + 1;
				short b = a + 1;
				short c = b + 1;
				short e = d + 1;
				short f = e + 1;
				if (run(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && run(validOpenZone[a], validOpenZone[b], validOpenZone[c]) && run(validOpenZone[d], validOpenZone[e], validOpenZone[f]))
				{
					if (validOpenZone[x].colour != validOpenZone[a].colour && validOpenZone[a].colour != validOpenZone[d].colour && validOpenZone[x].colour != validOpenZone[d].colour)
					{
						if (validOpenZone[x].value == validOpenZone[a].value && validOpenZone[x].value == validOpenZone[d].value)
						{
							han += 2;
							handTypes.push_back("Tricolour Run");
							closeTricolourRunLoop = true;
							break;
						}
					}
				}
			}
		}
	}

	// Check for Seven Pairs hand
	if (sevenPairs(validOpenZone))
	{
		han+=2;
		handTypes.push_back("Seven Pairs");
	}

	// Check for Pure Straight hand
	bool closePureStraightLoop = false;
	for (short x = 0; x < short(validOpenZone.size()) - 8; x++)
	{
		for (short x = 2; x < short(validOpenZone.size()) - 8; x += 3)
		{
			if (closePureStraightLoop)
			{
				break;
			}
			for (short a = x + 3; a < short(validOpenZone.size()) - 5; a += 3)
			{
				if (closePureStraightLoop)
				{
					break;
				}
				for (short d = a + 3; d < short(validOpenZone.size()) - 2; d += 3)
				{
					short y = x + 1;
					short z = y + 1;
					short b = a + 1;
					short c = b + 1;
					short e = d + 1;
					short f = e + 1;
					if (run(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && run(validOpenZone[a], validOpenZone[b], validOpenZone[c]) && run(validOpenZone[d], validOpenZone[e], validOpenZone[f]))
					{
						if (validOpenZone[x].colour == validOpenZone[a].colour && validOpenZone[a].colour == validOpenZone[d].colour)
						{
							if (validOpenZone[x].value == (validOpenZone[a].value + 3) && validOpenZone[x].value == (validOpenZone[d].value + 6))
							han += 2;
							handTypes.push_back("Pure Straight");
							closePureStraightLoop = true;
							break;
						}
					}
				}
			}
		}
		
	}

	// Check for Terminals and Honors hand
	short terminalsAndHonorsCount = 0;
	if (validOpenZone[0].value == 0 || validOpenZone[0].value == 1 || validOpenZone[0].value == 9)
	{
		for (short x = 2; x < short(validOpenZone.size()) - 2; x += 3)
		{
			short y = x + 2;
			if (validOpenZone[x].value == 1 || validOpenZone[x].value == 0 || validOpenZone[y].value == 9)
			{
				terminalsAndHonorsCount++;
			}
		}
		if (terminalsAndHonorsCount == 4)
		{
			han += 2;
			handTypes.push_back("Terminals and Honors");
		}
	}

	// Check for Three Hidden Triplets and Four Hidden Triplets hand
	if (hand.handOpen == false)
	{
		short hiddenTripletsCount = 0;
		for (short x = 2; x < short(validOpenZone.size()) - 8; x += 3)
		{
			short y = x + 1;
			short z = y + 1;
			if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]))
			{
				hiddenTripletsCount++;

			}
		}
		if (hiddenTripletsCount == 1)
		{
			han += 2;
			handTypes.push_back("Three Hidden Triplets");
		}
		if (hiddenTripletsCount == 2)
		{
			han += 8;
			handTypes.push_back("Four Hidden Triplets");
		}
	}

	// Check for Small Three Dragons hand
	bool closeSmallDragonsLoop = false;
	for (short x = 2; x < short(validOpenZone.size()) - 5; x += 3)
	{
		if (closeSmallDragonsLoop)
		{
			break;
		}
		for (short a = x + 3; a < short(validOpenZone.size()) - 2; a += 3)
		{
			short y = x + 1;
			short z = y + 1;
			short b = a + 1;
			short c = b + 1;
			if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && triple(validOpenZone[a], validOpenZone[b], validOpenZone[c]))
			{
				if (validOpenZone[x].type == "dragon" && validOpenZone[a].type == "dragon" && validOpenZone[0].type == "dragon")
				{
					if (validOpenZone[x].colour != validOpenZone[a].colour && validOpenZone[x].colour != validOpenZone[0].colour && validOpenZone[a].colour != validOpenZone[0].colour)
					han += 2;
					handTypes.push_back("Small Three Dragons");
					closeSmallDragonsLoop = true;
					break;
				}
			}
		}
	}

	// Check for Pure Terminals and Honors hand
	short pureTerminalsAndHonorsCount = 0;
	for (short x = 0; x < short(validOpenZone.size()); x++)
	{
		if (validOpenZone[x].value == 0 || validOpenZone[x].value == 1 || validOpenZone[x].value == 9)
		{
			pureTerminalsAndHonorsCount++;
		}

		if (pureTerminalsAndHonorsCount == 14)
		{
			han += 2;
			handTypes.push_back("Pure Honors and Terminals");
			break;
		}
	}

	// Check for Tricolour Triplets hand
	bool closeTricolourTripletsLoop = false;
	for (short x = 2; x < short(validOpenZone.size()) - 8; x += 3)
	{
		if (closeTricolourTripletsLoop)
		{
			break;
		}
		for (short a = x + 3; a < short(validOpenZone.size()) - 5; a += 3)
		{
			if (closeTricolourTripletsLoop)
			{
				break;
			}
			for (short d = a + 3; d < short(validOpenZone.size()) - 2; d += 3)
			{
				short y = x + 1;
				short z = y + 1;
				short b = a + 1;
				short c = b + 1;
				short e = d + 1;
				short f = e + 1;
				if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && triple(validOpenZone[a], validOpenZone[b], validOpenZone[c]) && triple(validOpenZone[d], validOpenZone[e], validOpenZone[f]))
				{
					if (validOpenZone[x].colour != validOpenZone[a].colour && validOpenZone[a].colour != validOpenZone[d].colour && validOpenZone[x].colour != validOpenZone[d].colour)
					{
						if (validOpenZone[x].value == validOpenZone[a].value && validOpenZone[x].value == validOpenZone[d].value)
						{
							if (validOpenZone[x].priorityOrder < 3 && validOpenZone[a].priorityOrder < 3 && validOpenZone[d].priorityOrder < 3)
							{
								han += 2;
								handTypes.push_back("Tricolour Triplets");
								closeTricolourTripletsLoop = true;
								break;
							}
							
						}
					}
				}
			}
		}
	}

	// Check for Half Flush and Full Flush hand
	short halfFlushCount = 0;
	short fullFlushCount = 0;
	for (short x = 0; x < short(validOpenZone.size()); x++)
	{
		if (validOpenZone[2].type == validOpenZone[x].type)
		{
			fullFlushCount++;
		}
		if (validOpenZone[2].type == validOpenZone[x].type || validOpenZone[x].value == 0)
		{
			halfFlushCount++;
		}
		if (fullFlushCount == 14)
		{
			han += 6;
			handTypes.push_back("Full Flush");
			break;
		}
		if (halfFlushCount == 14)
		{
			han += 3;
			handTypes.push_back("Half Flush");
			break;
		}
	}

	// Check for Terminal Foundations hand
	if (validOpenZone[0].value == 1 || validOpenZone[0].value == 9)
	{
		short terminalFoundationCount = 0;
		for (short x = 2; x < short(validOpenZone.size()) - 2; x += 3)
		{
			short y = x + 2;
			if (validOpenZone[x].value == 1 || validOpenZone[y].value == 9)
			{
				terminalFoundationCount++;
			}
			
			if (terminalFoundationCount == 4)
			{
				han += 3;
				handTypes.push_back("Terminal Foundations");
				break;
			}
		}
	}

	// Check for Thirteen Orphans hand
	if (thirteenOrphans(validOpenZone))
	{
		han += 8;
		handTypes.push_back("Thirteen Orphans");
	}

	// Check for Large Three Dragons hand
	bool closeThreeDragonsLoop = false;
	for (short x = 2; x < short(validOpenZone.size()) - 8; x += 3)
	{
		if (closeThreeDragonsLoop)
		{
			break;
		}
		for (short a = x + 3; a < short(validOpenZone.size()) - 5; a += 3)
		{
			if (closeThreeDragonsLoop)
			{
				break;
			}
			for (short d = a + 3; d < short(validOpenZone.size()) - 2; d += 3)
			{
				short y = x + 1;
				short z = y + 1;
				short b = a + 1;
				short c = b + 1;
				short e = d + 1;
				short f = e + 1;
				if (triple(validOpenZone[x], validOpenZone[y], validOpenZone[z]) && triple(validOpenZone[a], validOpenZone[b], validOpenZone[c]) && triple(validOpenZone[d], validOpenZone[e], validOpenZone[f]))
				{
					if (validOpenZone[x].colour != validOpenZone[a].colour && validOpenZone[a].colour != validOpenZone[d].colour && validOpenZone[x].colour != validOpenZone[d].colour)
					{
						if (validOpenZone[x].value == validOpenZone[a].value && validOpenZone[x].value == validOpenZone[d].value && validOpenZone[x].type == validOpenZone[a].type && validOpenZone[x].type == validOpenZone[d].type)
						{
							if (validOpenZone[x].type == "dragon")
							{
								han += 8;
								handTypes.push_back("Large Three Dragons");
								closeThreeDragonsLoop = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	// Check for Four Big Winds hand
	short fourWindsCount = 0;
	if (validOpenZone[2].type == "wind")
	{
		if (validOpenZone[5].type == "wind" && validOpenZone[8].type == "wind" && validOpenZone[11].type == "wind")
		{
			han += 8;
			handTypes.push_back("Big Four Winds");
		}
	}

	// Check for Small Four Winds hand
	bool closeSmallWindsLoop = false;
	if (validOpenZone[0].type == "wind")
	{
		for (short x = 2; x < short(validOpenZone.size()) - 8; x += 3)
		{
			if (closeSmallWindsLoop)
			{
				break;
			}
			for (short y = x + 3; y < short(validOpenZone.size()) - 5; y += 3)
			{
				if (closeSmallWindsLoop)
				{
					break;
				}

				for (short z = y + 3; z < short(validOpenZone.size()) - 2; z += 3)
				{
					if (validOpenZone[x].type == "wind" && validOpenZone[y].type == "wind" && validOpenZone[z].type == "wind")
					{
						han += 8;
						handTypes.push_back("Big Four Winds");
						closeSmallWindsLoop = true;
						break;
					}
				}
			}
		}
	}

	// Check for Pure Honors hand
	short pureHonorsCount = 0;
	for (short x = 0; x < short(validOpenZone.size()); x++)
	{
		if (validOpenZone[x].value == 0)
		{
			pureHonorsCount++;
		}
		if (pureHonorsCount == 14)
		{
			han += 8;
			handTypes.push_back("Pure Honors");
			break;
		}
	}

	// Check for Pure Terminals hand
	short pureTerminalsCount = 0;
	for (short x = 0; x < short(validOpenZone.size()); x++)
	{
		if (validOpenZone[x].value == 1 || validOpenZone[x].value == 9)
		{
			pureTerminalsCount++;
		}
		if (pureTerminalsCount == 14)
		{
			han += 8;
			handTypes.push_back("Pure Terminals");
			break;
		}
	}
	
	// Check for Pure Green hand
	short pureGreenCount = 0;
	for (short x = 0; x < short(validOpenZone.size()); x++)
	{
		if (validOpenZone[x].value == 0 || validOpenZone[x].value == 2 || validOpenZone[x].value == 3 || validOpenZone[x].value == 4 || validOpenZone[x].value == 6 || validOpenZone[x].value == 8)
		{
			if (validOpenZone[x].colour == 32)
			{
				pureGreenCount++;
			}
			
		}
		if (pureGreenCount == 14)
		{
			han += 8;
			handTypes.push_back("Pure Green");
			break;
		}
	}

	// Check for Nine Needs hand
	if (nineNeeds(validOpenZone))
	{
		han += 8;
		handTypes.push_back("Nine Needs");
	}

	if (han > 0)
	{
		handTrue = true;
	}

	/*
	cout << endl;
	for (short x = 0; x < handTypes.size(); x++)
	{
		cout << handTypes[x] << endl;
	}
	*/
}

void Player::checkTenpai(vector<Tile> tenpaiTiles)
{
	// Stupid tile creation require tile code from Deck class
	Tile tempTile;
	// List of tiles to check against
	vector <Tile> checkTiles;
	// Creates 1 of each tile to check for tenpai against
	for (short x = 1; x < 10; x++)
	{
		checkTiles.push_back(tempTile.CreateTile("red", x));
		checkTiles.push_back(tempTile.CreateTile("blue", x));
		checkTiles.push_back(tempTile.CreateTile("green", x));
	}
	checkTiles.push_back(tempTile.CreateTile("dragon", "white"));
	checkTiles.push_back(tempTile.CreateTile("dragon", "green"));
	checkTiles.push_back(tempTile.CreateTile("dragon", "red"));
	checkTiles.push_back(tempTile.CreateTile("wind", "north"));
	checkTiles.push_back(tempTile.CreateTile("wind", "south"));
	checkTiles.push_back(tempTile.CreateTile("wind", "east"));
	checkTiles.push_back(tempTile.CreateTile("wind", "west"));

	// If there are 2 tiles left
	if (tenpaiTiles.size() == 2)
	{
		// For each tile there is
		for (short x = 0; x < short(checkTiles.size()); x++)
		{
			// If it completes a run or triple
			if (run(tenpaiTiles[0], tenpaiTiles[1], checkTiles[x]) || triple(tenpaiTiles[0], tenpaiTiles[1], checkTiles[x]))
			{
				// Set Tenpai to true
				// Now I think about it, I don't think Tenpai has functionality outside of just calculating if it's true
				// Ususally it's used in tie situations but I didn't implement that function
				inTenpai = true;
				// Breaks out of function
				break;
			}
		}
	}
}


bool Player::pair(Tile x, Tile y)
{
	// If their value and type is the same
	if ((x.value == y.value) && (x.type == y.type))
	{
		// They are a pair
		return true;
	}
	// Otherwise, they aren't
	return false;
}

bool Player::triple(Tile x, Tile y, Tile z)
{
	// If their value and "type" is the same
	// Couldn't do type since wind and dragons share the same type but different tiles
	// Not a problem with pairs since you cannot send a pair to the open zone
	// Or runs since they have different values while honour tiles don't
	// Major design flaw in the tile system
	if ((x.value == y.value && x.value == z.value) && (x.priorityOrder == y.priorityOrder && x.priorityOrder == z.priorityOrder))
	{
		// They are a triple
		return true;
	}
	return false;
}

bool Player::run(Tile x, Tile y, Tile z)	// Bool function to check for a run
{
	// If their values increase by 1 and "type" is the same
	if (x.value == y.value-1 && x.value == z.value-2 && x.type == y.type && x.type == z.type)
	{
		// They are a run
		return true;
	}
	// Otherwise, they aren't
	return false;
}

bool Player::sevenPairs(vector<Tile> check)
{
	// Sorts the hand to prepare for checking pairs
	sort(check.begin(), check.end());
	short handMax = check.size();
	// If they have an odd number of tiles
	if (handMax % 2 > 0)
	{
		// Break out with false before any checking can cause an out of range error
		return false;
	}
	// For every 2 tiles in the hand
	for (short x = 0; x < handMax - 1; x += 2)
	{
		// tile y is 1 above tile x
		short y = x + 1;
		// Not sure why this is here, out of range prevention I guess?
		if (x < handMax - 1)
		{
			// If the tiles are not pairs
			// Not sure why Pair() isn't used, maybe I wanted to guarantee a pair since this check is important to not break the code
			if (!((check[x].value == check[(y)].value) && (check[x].type == check[(y)].type) && (check[x].priorityOrder == check[(y)].priorityOrder)))
			{
				// Return false
				return false;
			}
		}
	}
	// If it reaches here, all the tiles are pairs so return true
	return true;
}

bool Player::thirteenOrphans(vector<Tile> check)	// Bool function to check for the dreaded 13 orphans hand
{
	// If there is not 14 tiles in hand
	if (short(check.size()) != 14)
	{
		// Return false before any checking can happen
		return false;
	}
	// Create a hand used to check against 13 Orphans
	vector<Tile> thirteenOrphans;
	// Stupid tile creation require tile code from Deck class
	Tile tempTile;
	// Adds 13 Orphans' required tiles to the check hand
	thirteenOrphans.push_back(tempTile.CreateTile("red", 1));
	thirteenOrphans.push_back(tempTile.CreateTile("red", 9));
	thirteenOrphans.push_back(tempTile.CreateTile("blue", 1));
	thirteenOrphans.push_back(tempTile.CreateTile("blue", 9));
	thirteenOrphans.push_back(tempTile.CreateTile("green", 1));
	thirteenOrphans.push_back(tempTile.CreateTile("green", 9));
	thirteenOrphans.push_back(tempTile.CreateTile("dragon", "white"));
	thirteenOrphans.push_back(tempTile.CreateTile("dragon", "red"));
	thirteenOrphans.push_back(tempTile.CreateTile("dragon", "green"));
	thirteenOrphans.push_back(tempTile.CreateTile("wind", "north"));
	thirteenOrphans.push_back(tempTile.CreateTile("wind", "south"));
	thirteenOrphans.push_back(tempTile.CreateTile("wind", "east"));
	thirteenOrphans.push_back(tempTile.CreateTile("wind", "west"));

	// For each tile in the check hand
	for (short x = 0; x < 13; x++)
	{
		// For each tile in the real hand
		for (short y = 0; y < short(check.size()); y++)
		{
			// If both the check hand and real hand have the same tile
			if (pair(thirteenOrphans[x],check[y]))
			{
				// Erases that tile from the real hand
				check.erase(check.begin() + y);
				// Continues to the next tile in the check hand
				break;
			}
		}
	}
	// If all the correct tiles got erased from the real hand
	if (short(check.size()) == 1)
	{
		// For each tile in the check hand
		for (short x = 0; x < 13; x++)
		{
			// Check if the odd tile out pairs with a tile from the check hand
			if (pair(thirteenOrphans[x], check[0]))
			{
				// If it is, 13 Orphans is achieved
				return true;
			}
		}
	}
	// If it reaches here, the check was unsuccessful so return false
	return false;
}

bool Player::nineNeeds(vector<Tile> check)
{
	// Same principle as Thirteen Orphans above but the hand must remain closed to achieve it
	if (hand.handOpen == true)
	{
		return false;
	}
	for (short x = 0; x < short(check.size())-1; x++)
	{
		short y = x + 1;
		if (check[y].priorityOrder != check[x].priorityOrder)
		{
			return false;
		}
	}
	vector<Tile> nineNeeds;
	Tile tempTile;
	nineNeeds.push_back(tempTile.CreateTile("red", 1));
	nineNeeds.push_back(tempTile.CreateTile("red", 1));
	nineNeeds.push_back(tempTile.CreateTile("red", 1));
	nineNeeds.push_back(tempTile.CreateTile("red", 2));
	nineNeeds.push_back(tempTile.CreateTile("red", 3));
	nineNeeds.push_back(tempTile.CreateTile("red", 4));
	nineNeeds.push_back(tempTile.CreateTile("red", 5));
	nineNeeds.push_back(tempTile.CreateTile("red", 6));
	nineNeeds.push_back(tempTile.CreateTile("red", 7));
	nineNeeds.push_back(tempTile.CreateTile("red", 8));
	nineNeeds.push_back(tempTile.CreateTile("red", 9));
	nineNeeds.push_back(tempTile.CreateTile("red", 9));
	nineNeeds.push_back(tempTile.CreateTile("red", 9));

	for (short x = 0; x < 13; x++)
	{
		for (short y = 0; y < short(check.size()); y++)
		{
			if (nineNeeds[x].value == check[y].value)
			{
				check.erase(check.begin() + y);
				break;
			}
		}
	}
	if (short(check.size()) == 1)
	{
		for (short x = 0; x < 13; x++)
		{
			if (nineNeeds[x].value == check[0].value)
			{
				return true;
			}
		}
	}
	return false;
}

bool Player::pon(Tile x)
{
	// For each 2 tiles in the hand
	for (short y = 0; y < short(hand.tiles.size()) - 1; y++)		
	{
		short z = y + 1;
		// If a triple is possible with the passed tile
		if (triple(x, hand.tiles[y], hand.tiles[z]))
		{
			// Pon can be called
			return true;
		}
	}
	// Otherwise, it can't
	return false;
}

void Player::sendPon(Tile x)
{
	// Finds the Pon from the code above
	for (short y = 0; y < short(hand.tiles.size()) - 1; y++)
	{
		short z = y + 1;
		if (triple(x, hand.tiles[y], hand.tiles[z]))
		{
			// Moves it to the open zone instead
			Tile moveY = hand.tiles[y];
			Tile moveZ = hand.tiles[z];
			hand.tiles.erase(hand.tiles.begin() + z);
			hand.tiles.erase(hand.tiles.begin() + y);
			hand.openZone.push_back(x);
			hand.openZone.push_back(moveY);
			hand.openZone.push_back(moveZ);
			hand.handOpen = true;
			// Breaks out
			return;
		}
	}
}

bool Player::chi(Tile x)
{
	// For each run combination
	// Inefficient again
	for (short y = 0; y < short(hand.tiles.size()) - 1; y++)		
	{
		for (short z = y + 1; z < short(hand.tiles.size()); z++)
		{
			// Chii can be called if a run can be achieved from any order of the 3 tiles
			// Again, runs suck and are inefficient
			if (run(x, hand.tiles[y], hand.tiles[z]))
			{
				return true;
			}
			if (run(hand.tiles[y], x, hand.tiles[z]))
			{
				return true;
			}
			if (run(hand.tiles[y], hand.tiles[z], x))
			{
				return true;
			}
		}
	}
	// Otherwise, it can't
	return false;
}

void Player::sendChi(Tile x)
{
	// Finds the Chi from the code above
	for (short y = 0; y < short(hand.tiles.size()) - 1; y++)		
	{
		for (short z = y + 1; z < short(hand.tiles.size()); z++)
		{
			// Moves it to the open zone instead
			if (run(x, hand.tiles[y], hand.tiles[z]))
			{
				Tile moveY = hand.tiles[y];
				Tile moveZ = hand.tiles[z];
				hand.tiles.erase(hand.tiles.begin() + z);
				hand.tiles.erase(hand.tiles.begin() + y);
				hand.openZone.push_back(x);
				hand.openZone.push_back(moveY);
				hand.openZone.push_back(moveZ);
				hand.handOpen = true;  
				return;
			}
			if (run(hand.tiles[y], x, hand.tiles[z]))
			{
				Tile moveY = hand.tiles[y];
				Tile moveZ = hand.tiles[z];
				hand.tiles.erase(hand.tiles.begin() + z);
				hand.tiles.erase(hand.tiles.begin() + y);
				hand.openZone.push_back(moveY);
				hand.openZone.push_back(x);
				hand.openZone.push_back(moveZ);
				hand.handOpen = true;
				return;
			}
			if (run(hand.tiles[y], hand.tiles[z], x))
			{
				Tile moveY = hand.tiles[y];
				Tile moveZ = hand.tiles[z];
				hand.tiles.erase(hand.tiles.begin() + z);
				hand.tiles.erase(hand.tiles.begin() + y);
				hand.openZone.push_back(moveY);
				hand.openZone.push_back(moveZ);
				hand.openZone.push_back(x);
				hand.handOpen = true;
				return;
			}
		}
	}
}