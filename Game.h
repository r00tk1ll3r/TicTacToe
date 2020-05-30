#ifndef GAME_H
#define GAME_H

#include "Board.h"

struct Move
{
	int row;
	int col;
};

class Game
{
private:
	Board m_board;
	bool m_gameOver;
	char m_turn;
	int gameStatus;

	bool m_humanMovePerformed;

	enum class Mode
	{
		Singleplayer = 0,
		Multiplayer = 1
	};
	Mode m_gameMode;

public:
	Game();
	void run(); // Main game loop

private:
	void menu();		// Game menu
	void userInput();	// Handles user input
	void logic();		// Game logic
	void switchTurns(); // Switches turn

	void gameRules();  // Self-descriptive
	void viewRecord(); // Dump the contents of record.txt to the console

	int evaluate(Board board);
	int minimax(Board board, int depth, bool isMax);
	Move findBestMove(Board board);
};

#endif
