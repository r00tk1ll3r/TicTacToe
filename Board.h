#ifndef BOARD_H
#define BOARD_H

#define GRID_SIZE 3
#define PLAYER 'X'
#define OPPONENT 'O'

class Board
{
public:
	char m_grid[GRID_SIZE][GRID_SIZE];

public:
	void genGrid();									// Generates grid
	void draw();									// Draws grid to the screen
	void clearScreen();								// Clear console
	bool changeGridValue(int location, char value); // Changes grid value based on user/computer input
	bool isVictory();								// Checks victory
	bool isTie();									// Checks tie
	char victoryTurn();								// Returns victory turn
};

#endif