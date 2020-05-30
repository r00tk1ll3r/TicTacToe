#include "Board.h"

#include <iostream>

// Generates grid
void Board::genGrid()
{
	char num = '1';

	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			m_grid[i][j] = num;
			num++;
		}
	}
}

void Board::clearScreen()
{
#if defined(_WIN32) // if the operating system is an i386 Windows
	system("CLS");
#elif defined(_WIN64) // if the operating system is an amd64 Windows
	system("CLS");
#endif

#if defined(__UNIX__) || (defined(__APPLE__) && defined(__MACH__)) // if the operating system is UNIX or MacOS (OSX)
#include <sys/param.h>
	system("clear");
#endif

#if defined(__linux__) // if the operating system runs on the Linux kernel
	system("clear");
#endif
}

// Draws grid to screen
void Board::draw()
{
	// Clears screen every turn
	clearScreen();

	std::cout << "\n -------------\n";

	for (int i = 0; i < GRID_SIZE; i++)
	{
		std::cout << " | ";
		for (int j = 0; j < GRID_SIZE; j++)
		{
			std::cout << m_grid[i][j] << " | ";
		}
		std::cout << "\n -------------\n";
	}
}

// Changes grid value based on user/ai input
bool Board::changeGridValue(int location, char value)
{
	// Formula for onverting 1-9 to row and column
	int row = (location - 1) / GRID_SIZE;
	int col = (location - 1) % GRID_SIZE;

	// Check if position is already taken
	if (m_grid[row][col] == PLAYER || m_grid[row][col] == OPPONENT)
		return false;

	// Sets value to X or O
	m_grid[row][col] = value;
	return true;
}

// Checks victory
bool Board::isVictory()
{
	const int totalWinningMoves = 8;
	const char *winningMoveSet[totalWinningMoves] = {"123", "456", "789", "147", "258", "369", "159", "357"};

	for (int i = 0; i < totalWinningMoves; i++)
	{
		bool winner = true;
		char previous = '0';
		const char *winningMove = winningMoveSet[i];

		for (int j = 0; j < GRID_SIZE; j++)
		{
			char character = winningMove[j];
			int location = (character - '0') - 1;

			int row = location / GRID_SIZE;
			int col = location % GRID_SIZE;

			char gridChar = m_grid[row][col];

			if (previous == '0')
			{
				previous = gridChar;
			}
			else if (previous == gridChar)
			{
				continue;
			}
			else
			{
				winner = false;
				break;
			}
		}
		if (winner)
			return winner;
	}
	return false;
}

// Returns victory turn
char Board::victoryTurn()
{
	const int totalWinningMoves = 8;
	const char *winningMoveSet[totalWinningMoves] = {"123", "456", "789", "147", "258", "369", "159", "357"};

	for (int i = 0; i < totalWinningMoves; i++)
	{
		bool winner = true;
		char previous = '0';
		const char *winningMove = winningMoveSet[i];

		for (int j = 0; j < GRID_SIZE; j++)
		{
			char character = winningMove[j];
			int location = (character - '0') - 1;

			int row = location / GRID_SIZE;
			int col = location % GRID_SIZE;

			char gridChar = m_grid[row][col];

			if (previous == '0')
			{
				previous = gridChar;
			}
			else if (previous == gridChar)
			{
				continue;
			}
			else
			{
				winner = false;
				break;
			}
		}
		if (winner)
			return previous;
	}
	return ' ';
}

// Checks tie
bool Board::isTie()
{
	bool tie = false;

	for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
	{
		int row = i / GRID_SIZE;
		int col = i % GRID_SIZE;

		if (m_grid[row][col] == PLAYER || m_grid[row][col] == OPPONENT)
		{
			tie = true;
		}
		else
		{
			tie = false;
			break;
		}
	}

	return tie;
}
