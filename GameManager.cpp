#include "GameManager.h"


void GameManager::startGame()
{
	// Intro text output
	cout << "Welcome to Budget Richii Majong!" << endl;
	cout << "There are no kans, dora or hands based on timed situations in this version" << endl;
	cout << "Inputs are made using UP arrow, DOWN arrow and SPACE" << endl;
	cout << "Press SPACE to start";
	getchLoop();
	// Starts 4 games with wind directions changing each time.
	// Also the "Deal Wins, Keep Dealer" rule is not implemented.
	startGame(0);
	startGame(1);
	startGame(2);
	startGame(3);
	endScreen();
	getchLoop();
}

short GameManager::selectMenu(string variant, short selectPlayer)
{
	// When the passed string is discard, ron, tsumo, chii, pon or richii create a selection menu from the info below

	// String vector used later
	vector <string> inputVariant;
	// Discard variant
	if (variant == "discard")
	{
		// The 1st element to inputVariant is the text that is displayed above the selection.
		// The other elements are the selection options
		// Unless the selection requires a hand to be shown, then "hand" is used and the sequence moves up 1.
		inputVariant.push_back("hand");
		inputVariant.push_back("Choose a tile to discard.");
		short chosenTile = selectMenu(inputVariant);
		// Discards the tile chosen in this variant
		discardTile = player[selectPlayer].hand.tiles[chosenTile];
		player[selectPlayer].hand.DiscardTile(chosenTile);
		return 1;
	}
	// Ron variant
	if (variant == "ron")
	{
		inputVariant.push_back("Call Ron?");
		inputVariant.push_back("Yes");
		inputVariant.push_back("No");
		// Returns the cursor position
		return selectMenu(inputVariant);
	}
	// Tsumo variant
	if (variant == "tsumo")
	{
		inputVariant.push_back("Call Tsumo?");
		inputVariant.push_back("Yes");
		inputVariant.push_back("No");
		return selectMenu(inputVariant);
	}
	// Chii variant
	if (variant == "chii")
	{
		inputVariant.push_back("Call Chii?");
		inputVariant.push_back("Yes");
		inputVariant.push_back("No");
		return selectMenu(inputVariant);
	}
	// Pon variant
	if (variant == "pon")
	{
		inputVariant.push_back("Call Pon?");
		inputVariant.push_back("Yes");
		inputVariant.push_back("No");
		return selectMenu(inputVariant);
	}
	// Richii variant
	if (variant == "richii")
	{
		inputVariant.push_back("Call Richii?");
		inputVariant.push_back("Yes");
		inputVariant.push_back("No");
		return selectMenu(inputVariant);
	}
	return 1;
}

short GameManager::selectMenu(vector <string> line)
{
	// A lot of code used to output and maintain the menu

	// Cursor used to return a value back
	short cursor = 0;
	// Used to calculate the length of the selection
	short lineSize = -1;
	// While loop
	bool selectLoop = true;
	while (selectLoop == true)
	{	
		// Display the state of the game
		displayGame();
		// If the hand is meant to be shown
		if (line[0] == "hand")
		{
			lineSize = 0;
			// Calculate lineSize based off the hand used
			for (short x = 0; x < player[getCurrentPlayer()].hand.tiles.size(); x++)
			{
				lineSize++;
			}
			// Outputs the question
			cout << line[1] << endl;
			// Outputs some lines
			cout << "===================" << endl;
			for (short x = 0; x < player[getCurrentPlayer()].hand.tiles.size(); x++)
			{
				// If the cursor highlights the line
				if (x == cursor)
				{
					// Adds a cursor to the line
					cout << "->    ";
				}
				// Outputs the line
				cout << " ";
				player[getCurrentPlayer()].hand.tiles[x].DisplayTile();
				cout << endl;
			}
		}
		else
		{
			// Same code but without the hand code.
			lineSize = -1;
			for (short x = 0; x < line.size(); x++)
			{
				lineSize++;
			}
			cout << line[0] << " ";
			// If chii or pon is being called
			if (line[0] == "Call Chii?" || line[0] == "Call Pon?")
			{
				// Display the tile being called on
				discardTile.DisplayTile();
			}
			cout << endl;
			cout << "===================" << endl;
			for (short x = 1; x < line.size(); x++)
			{
				if (x == cursor + 1)
				{
					cout << "->    ";
				}
				cout << " " << line[x] << endl;
			}
		}

		// Different Getch loop
		char key_press = _getch();
		short ascii = key_press;
		switch (ascii)
		{
		// Down Arrow
		case 80:
			// Wrap Around Cursor Code. Yea
			if (cursor+1 == lineSize)
			{
				cursor = -1;
			}
			// Clamps cursor to selection
			cursor = clamp(cursor + 1, 0, lineSize - 1);
			break;
		// Up Arrow
		case 72:
			if (cursor-1 < 0)
			{
				cursor = lineSize;
			}
			cursor = clamp(cursor - 1, 0, lineSize - 1);
			break;
		// Space
		case 32:
			// Breaks loop
			selectLoop = false;
			break;
		}
		// Clear console
		system("CLS");
	}
	// Returns cursor position
	return cursor;
}

short GameManager::getCurrentPlayer()
{
	return currentPlayer;
}

short GameManager::setCurrentPlayer(short newNumber)
{
	// Wrap arround mechanism
	if (newNumber == 4)
	{
		currentPlayer = 0;
	}
	else
	{
		currentPlayer = newNumber;
	}
	return 0;
}

Tile GameManager::giveTile(short drawPlayer)
{
	// True Random Number Generator
	random_device generator;
	uniform_int_distribution<short> distribution(0, deck.deckSize() - 1);
	// Chooses a tile randomly
	short tileChosen = distribution(generator);
	Tile tile = deck.tiles[tileChosen];
	// Gives the player that tile then erases it from the deck
	player[drawPlayer].hand.tiles.push_back(tile);
	deck.tiles.erase(deck.tiles.begin()+tileChosen);
	// Updates the deck counter
	deck.reduceDeckSize();
	// Returns the drawn tile to be used for identifying later
	return tile;
}

void GameManager::initialDraw()
{
	// Simple for loops to draw 13 tiles to each hand
	for (short y = 0; y < 4; y++)
	{
		for (short x = 0; x < 13; x++)
		{
			giveTile(y);
		}
		// Sorts the hand
		player[y].hand.SortHand();
	}
	
}

void GameManager::playerControl(short controlledPlayer)
{
	// Speaks for itself
	setCurrentPlayer(controlledPlayer);
	cout << "Player " << controlledPlayer + 1 << ", please press SPACE.";
	getchLoop();
	system("CLS");
}

void GameManager::displayGame()
{
	// For each player
	for (short x = 0; x < 4; x++)
	{
		// Outputs their name
		cout << "Player " << x+1 << ": ";
		// If they are the current player
		if (x == currentPlayer)
		{
			// Reveal their hand in the display
			for (short y = 0; y < short(player[x].hand.tiles.size()); y++)
			{
				player[x].hand.tiles[y].DisplayTile();
				cout << " ";
			}
		}
		// If not
		else
		{	
			// Tiles shown are blank red tiles
			Tile blankTile;
			blankTile.display = " ";
			blankTile.colour = 64;
			for (short y = 0; y < short(player[x].hand.tiles.size()); y++)
			{
				blankTile.DisplayTile();
				cout << " ";
			}
			cout << "  ";
		}

		// Outputs the players open zone
		cout << "     ";
		for (short y = 0; y < short(player[x].hand.openZone.size()); y++)
		{
			player[x].hand.openZone[y].DisplayTile();
			cout << " ";
		}

		// Outputs the players discard pile
		cout << "     Discard Pile: ";
		for (short y = 0; y < short(player[x].hand.discardPile.size()); y++)
		{
			player[x].hand.discardPile[y].DisplayTile();
			cout << " ";
		}
		
		// Outputs the players direrction
		cout << "     Direction: " << player[x].hand.direction;
		// If the player is in Richii
		if (player[x].inRichi == true)
		{
			// Outputs this info
			cout << "     Richi!";
		}
		cout << endl;
	}
	// Outputs the deck state and display lines
	cout << "There are " << deck.deckSize() << " tiles remaining in the deck." << endl;
	cout << "-------------------------------------------------------------------------------------------------------" << endl;
}

void GameManager::displayPoints()
{
	// Displays each players points and display lines
	for (short x = 0; x < 4; x++)
	{
		cout << "Player " << x + 1 << ": " << player[x].points << endl;
	}
	cout << "------------------" << endl;
}

void GameManager::startGame(short eastPlayer)
{
	// Creates a list of the 4 directions
	string directions[4];
	directions[0] = "E";
	directions[1] = "S";
	directions[2] = "W";
	directions[3] = "N";
	// Sets the current player to the passed value
	short x = eastPlayer;
	setCurrentPlayer(x);
	// Sets the direction of each player according to the list
	for (short y = 0; y < 4; y++)
	{
		// Wrap arround code
		if (x == 4)
		{
			x = 0;
		}
		player[x].hand.SetDirection(directions[y]);
		x++;
	}
	// Starts the game
	gameLoop();
	// Once the game is done, all variables get reset
	deck.tiles.clear();
	for (short x = 0; x < 4; x++)
	{
		player[x].hand.tiles.clear();
		player[x].hand.openZone.clear();
		player[x].hand.discardPile.clear();
		player[x].hand.handOpen = false;
		player[x].han = 0;
		player[x].handValid = false;
		player[x].handTrue = false;
		player[x].inRichi = false;
		player[x].inTenpai = false;
		player[x].handTypes.clear();
	}
	deck.setDeckSize(136);
}

void GameManager::gameLoop()
{
	// Add all the tiles into the deck
	deck.createDeck();
	// Draws the tiles to players hands
	initialDraw();
	// Sets up some variables
	bool gamePlaying = true;
	bool skipDraw = false;
	Tile newTile;
	short playerTurn;
	// While loop
	while (gamePlaying)
	{
		// Add a variable to keep track of the current player's turn
		playerTurn = currentPlayer;
		// Give control to that player
		playerControl(currentPlayer);
		
		// Draw Tile to Player and assign it to a variable
		if (skipDraw == false)
		{
			newTile = giveTile(currentPlayer);
			displayGame();
			cout << "You drew a ";
			newTile.DisplayTile();
			getchLoop();
		}

		// Validates Players Hand
		player[currentPlayer].validateHand();

		// Tsumo Check
		if (player[currentPlayer].handTrue == true && skipDraw == false)
		{
			// Tsumo Select menu
			if (selectMenu("tsumo", currentPlayer) == 0)
			{
				// If confirmed, call tsumo and end the game
				tsumo(currentPlayer);
				gamePlaying = false;
				continue;
			}
		}


		// Resets the skip draw rule
		if (skipDraw == true)
		{
			skipDraw = false;
		}

		// Richi Discard
		if (player[currentPlayer].inRichi == true)
		{
			// If the player is in Richii, automatically discard the new tile drawn
			for (short x = 0; x < short(player[currentPlayer].hand.tiles.size()); x++)
			{
				if (player[currentPlayer].pair(newTile, player[currentPlayer].hand.tiles[x]))
				{
					displayGame();
					cout << "You discarded ";
					discardTile = player[currentPlayer].hand.tiles[x];
					player[currentPlayer].hand.tiles[x].DisplayTile();
					player[currentPlayer].hand.DiscardTile(x);
					getchLoop();
					break;
				}
			}
			// Check if any players can call on the discarded tile.
			if (ronCheck(playerTurn))
			{
				// Ends the game
				gamePlaying = false;
				continue;
			}
			if (chiCheck())
			{
				// Removes the tile from the discard pile
				player[playerTurn].hand.discardPile.pop_back();
				// That player then skips a draw
				skipDraw = true;
				// Make it that players turn
				continue;
			}
			// Resets the current player back if it fails
			setCurrentPlayer(playerTurn);
		}

		// Normal Discard
		if (player[currentPlayer].inRichi == false)
		{
			// Discard select menu that automatically discards a tile
			selectMenu("discard", currentPlayer);
			// Validates hand for richii check
			player[currentPlayer].validateHand();
			// Check if richii can be called
			if (player[currentPlayer].inTenpai && player[currentPlayer].hand.handOpen == false)
			{
				if (selectMenu("richii", currentPlayer) == 0)
				{
					// If confirmed, call richii
					player[currentPlayer].inRichi = true;
				}
			}
			// Same discard checks as before
			if (ronCheck(playerTurn))
			{
				gamePlaying = false;
				continue;
			}
			if (chiCheck())
			{
				player[playerTurn].hand.discardPile.pop_back();
				skipDraw = true;
				continue;
			}
			setCurrentPlayer(playerTurn);
		}
		
		// Quits if deck is empty
		if (deck.deckSize() == 0)
		{
			system("CLS");
			displayPoints();
			cout << "No winners this round.";
			getchLoop();
			gamePlaying = false;
			continue;
		}

		// Continues the game normally
		setCurrentPlayer(currentPlayer + 1);
		continue;
	}
}

bool GameManager::chiCheck()
{
	short chiPlayer = currentPlayer;
	// For every other player
	for (short x = 1; x < 4; x++)
	{
		// Wrap around code
		chiPlayer++;
		if (chiPlayer == 4)
		{
			chiPlayer = 0;
		}

		// If the player is not in richii
		if (player[chiPlayer].inRichi == false)
		{
			// If they are the player to the left
			if (x == 3)
			{
				// Check if chii can be called
				if (player[chiPlayer].chi(discardTile))
				{
					// Gives control to the player if they can
					playerControl(chiPlayer);
					// Chii select menu
					if (selectMenu("chii", chiPlayer) == 0)
					{
						// If confirmed, call chii
						player[chiPlayer].sendChi(discardTile);
						// Returns true if a call is made
						return true;
					}
				}
			}

			// Check if pon can be called
			if (player[chiPlayer].pon(discardTile))
			{
				// Same code as chii
				playerControl(chiPlayer);
				if (selectMenu("pon", chiPlayer) == 0)
				{
					player[chiPlayer].sendPon(discardTile);
					return true;
				}
			}
		}
		
		
	}
	// Returns false if no call is made
	return false;
}

void GameManager::tsumo(short winPlayer)
{
	// Calculates the points that the winner will take
	int stealPoints = player[winPlayer].han * 2000;
	if (player[winPlayer].han >= 8)
	{
		stealPoints += 8000;
	}
	// Adds the points taken to the winning player
	player[winPlayer].points += stealPoints;
	// For every other player
	for (short x = 1; x < 4; x++)
	{
		// Lose 1/3 of what the winning player gained
		if (winPlayer + x > 3)
		{
			player[winPlayer + x - 4].points -= (stealPoints / 3);
		}
		else
		{
			player[winPlayer + x].points -= (stealPoints / 3);
		}
	}
	system("CLS");
	// Displays the points of all the players and shows how the player won
	displayGame();
	displayPoints();
	cout << "TSUMO! Player " << winPlayer + 1 << " wins this round." << endl;
	for (short x = 0; x < player[winPlayer].handTypes.size(); x++)
	{
		cout << player[winPlayer].handTypes[x] << endl;
	}
	cout << "Press SPACE to start the next round!";
	getchLoop();
}

void GameManager::ron(short winPlayer, short deadPlayer)
{
	// Fixes Closed Tsumo being shown on Ron
	for (short x = 0; x < player[winPlayer].handTypes.size(); x++)
	{
		if (player[winPlayer].handTypes[x] == "Closed Tsumo")
		{
			player[winPlayer].handTypes.erase(player[winPlayer].handTypes.begin() + x);
			player[winPlayer].han -= 1;
		}
	}
	// Calculates the points that the winner will take
	int stealPoints = player[winPlayer].han * 2000;
	if (player[winPlayer].han >= 8)
	{
		stealPoints += 8000;
	}
	// Adds the points taken to the winning player
	player[winPlayer].points += stealPoints;
	// Takes all the points away from the person who allowed them to win
	player[deadPlayer].points -= stealPoints;
	system("CLS");
	// Displays the points of all the players and shows how the player won
	displayGame();
	displayPoints();
	cout << "RON! Player " << winPlayer + 1 << " destroys Player " << deadPlayer + 1 << "." << endl;
	for (short x = 0; x < player[winPlayer].handTypes.size(); x++)
	{
		cout << player[winPlayer].handTypes[x] << endl;
	}
	cout << "Press SPACE to start the next round!";
	getchLoop();
}

bool GameManager::ronCheck(short turnPlayer)
{
	// For every other player
	short ronPlayer = currentPlayer;
	for (short x = 1; x < 4; x++)
	{
		ronPlayer++;
		if (ronPlayer == 4)
		{
			ronPlayer = 0;
		}

		// Duplicates the player
		Player testPlayer = player[ronPlayer];
		// Adds the discarded tile to their hand
		testPlayer.hand.tiles.push_back(discardTile);
		// Validates the hand
		testPlayer.validateHand();
		// If the hand can win from the tile given
		if (testPlayer.handTrue == true)
		{
			// Gives control to the player
			playerControl(ronPlayer);
			// Ron select menu
			if (selectMenu("ron", ronPlayer) == 0)
			{
				// If confiirmed, call ron and returns true for a success
				player[ronPlayer].hand.tiles.push_back(discardTile);
				player[ronPlayer].validateHand();
				ron(ronPlayer, turnPlayer);
				return true;
			}
		}
	}
	// Resets the current player and returns false for no calls
	setCurrentPlayer(turnPlayer);
	return false;
}

void GameManager::getchLoop()
{
	// The getch loop was inspired by the Island Adventure workshop.

	// While loop
	bool loop = true;
	while (loop)
	{
 	 	char key_press = _getch();
		short ascii = key_press;
		// If SPACE is pressed, break loop. If not, wait
		if (ascii == 32) 
		{
			loop = false;
		}
	}
	// Clear Console
	system("CLS"); 
}

void GameManager::endScreen()
{
	// Shows points then outputs the winner
	displayPoints();
	// Player 1 Win
	if (player[0].points > player[1].points && player[0].points > player[2].points && player[0].points > player[3].points)
	{
		cout << "Player 1 Wins!";
		return;
	}
	// Player 2 Win
	if (player[1].points > player[0].points && player[1].points > player[2].points && player[1].points > player[3].points)
	{
		cout << "Player 2 Wins!";
		return;
	}
	// Player 3 Win
	if (player[2].points > player[0].points && player[2].points > player[1].points && player[2].points > player[3].points)
	{
		cout << "Player 3 Wins!";
		return;
	}
	// Player 4 Win
	if (player[3].points > player[0].points && player[2].points > player[1].points && player[3].points > player[2].points)
	{
		cout << "Player 4 Wins!";
		return;
	}

	// Reached if all other ifs fail
	cout << "It's a Tie!";
}

GameManager::GameManager()
{
	// Give it a default value or else VS panics
	currentPlayer = 0;
}

