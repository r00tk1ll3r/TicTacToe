#include "Game.h"

#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <chrono>

Game::Game()
	: m_gameOver(false), m_turn(PLAYER), m_humanMovePerformed(true)
{
	m_board.genGrid();
}

// Game menu
void Game::menu()
{
	std::cout << "Tic Tac Toe!\n";
	std::cout << "------------\n";

	int choice;

	while (true)
	{
		std::cout << "1) play game!\n";
		std::cout << "2) read rules!\n";
		std::cout << "3) view record!\n";
		std::cout << "4) quit game!\n";
		std::cout << "\nYour Choice : ";
		std::cin >> choice;

		m_board.clearScreen();

		if (choice == 1)
		{
			while (true)
			{
				std::cout << "Tic Tac Toe!\n";
				std::cout << "------------\n";

				int modeChoice;
				std::cout << "1) play single player!\n";
				std::cout << "2) play multi player!\n";
				std::cout << "\nYour Choice : ";
				std::cin >> modeChoice;

				if (modeChoice == 1)
				{
					m_gameMode = Mode::Singleplayer;
					break;
				}
				else if (modeChoice == 2)
				{
					m_gameMode = Mode::Multiplayer;
					break;
				}
				else
				{
					std::cout << "You have to enter a value between 1 - 2";
				}
			}
			break;
		}

		else if (choice == 2)
		{
			gameRules();
			menu();
			break;
		}
		else if (choice == 3)
		{
			viewRecord();
			menu();
			break;
		}
		else if (choice == 4)
		{
			std::cout << "\nThanks for playing!\n";
			m_gameOver = true;
			break;
		}
		else
		{
			std::cout << "You have to enter a value between 1 - 4";
		}
	}
}

void Game::gameRules()
{
	m_board.clearScreen();
	std::cout << "Rules of the Tic-Tac-Toe game\n";
	std::cout << "-----------------------------\n\n";
	std::cout << " -- The game is to be played between two people (HUMAN vs HUMAN or HUMAN vs COMPUTER depending on your choice).\n";
	std::cout << " -- One of the player chooses 'O' and the other 'X' to mark their respective cells.\n";
	std::cout << " -- The game starts with one of the players and the game ends when one of the players has one whole row/column/diagonal filled with their respective character ('O' or 'X').\n";
	std::cout << " -- If no one wins, the game is said to be draw.\n";
	std::cout << "-------------\n"
			  << "| O | X | O |\n"
			  << "-------------\n"
			  << "| O | X | X |\n"
			  << "-------------\n"
			  << "| X | O | X |\n"
			  << "-------------\n";
	std::cout << "\nPress Enter to return.";
	std::cin.ignore();
	std::cin.get();

	return;
}

void Game::viewRecord()
{
	std::cout << "Gameplay Records\n";
	std::cout << "----------------\n\n";
	char buff[100];
	std::ifstream in;
	in.open("record.txt", std::ios::in);
	if (!in)
	{
		std::cout << "Cannot open file. Please make sure the file exists and is not being used by another process. Please ignore this message if this is your first gameplay.\n";
		return;
	}
	in.seekg(0);
	while (in)
	{
		in.getline(buff, 100);
		std::cout << buff << "\n";
	}
	in.close();
	std::cout << "\nPress Enter to return.";
	std::cin.ignore();
	std::cin.get();

	return;
}

// Main game loop
void Game::run()
{
	auto start = std::chrono::system_clock::now(); // current time recorded
	char opt = 'y';
	menu();
	do
	{
		m_gameOver = false;
		m_humanMovePerformed = true;
		m_turn = PLAYER;
		m_board.genGrid();
		while (!m_gameOver)
		{
			m_board.draw();
			userInput();
			logic();
		}
		std::cout << "\nDo You Want to Play Again [y|n] ? ";
		std::cin >> opt;
	} while (opt == 'y');

	auto end = std::chrono::system_clock::now(); // current time recorded

	// evaluate duration of gameplay by subtracting start time from end time
	std::chrono::duration<double> elapsed_seconds = end - start;

	// convert start to data-type time_t; same for end
	std::time_t start_time = std::chrono::system_clock::to_time_t(start);
	std::ofstream out;
	out.open("record.txt", std::ios::app | std::ios::out);
	out << "User started playing at " << std::ctime(&start_time) << "Gameplay duration: " << elapsed_seconds.count() << "s\n";
	switchTurns();
	if (m_gameMode == Mode::Singleplayer)
		out << "Game Mode : Singleplayer\n";
	else
		out << "Game Mode : Multiplayer\n";
	if (gameStatus == 1)
		out << "Game Result : " << m_turn << " Win\n\n";
	else
		out << "Game Result : Draw\n\n";
	out.close();
}

// Handles user input
void Game::userInput()
{
	int input;

	while (true)
	{
		std::cout << "\nIt is " << m_turn << "'s turn";
		std::cout << "\nWhere would you like to play? : ";
		std::cin >> input;

		if (input >= 1 && input <= 9)
			break;

		std::cout << "You have to enter a value between 1 - 9";
		std::cin.clear();
		std::cin.ignore();
	}

	if (!m_board.changeGridValue(input, m_turn))
	{
		std::cout << "That position is already taken!\n";
		m_humanMovePerformed = false;

		std::cin.ignore();
		std::cin.get();

		switchTurns();
	}
	else
	{
		m_humanMovePerformed = true;
	}
}

// Game logic
void Game::logic()
{
	// Check victory
	if (m_board.isVictory())
	{
		m_board.draw();
		std::cout << "\nThe Winner is " << m_turn;
		m_gameOver = true;
		gameStatus = 1;
	}

	// Check tie
	if (m_board.isTie() && !m_gameOver)
	{
		std::cout << "\nGame Draw!\n";
		m_gameOver = true;
		gameStatus = 0;
	}
	switchTurns();

	if (m_gameMode == Mode::Singleplayer && !m_gameOver && m_humanMovePerformed)
	{
		Move bestMove = findBestMove(m_board);
		int location = ((bestMove.row) * GRID_SIZE) + (bestMove.col + 1);

		m_board.changeGridValue(location, m_turn);

		if (m_board.isVictory())
		{
			m_board.draw();
			std::cout << "\nThe Winner is " << m_turn;
			m_gameOver = true;
			gameStatus = 1;
		}

		switchTurns();
	}
}

// Switches turn
void Game::switchTurns()
{
	if (m_turn == PLAYER)
		m_turn = OPPONENT;
	else
		m_turn = PLAYER;
}

int Game::evaluate(Board board)
{

	if (board.isVictory() && board.victoryTurn() == OPPONENT)
		return +10;
	else if (board.isVictory() && board.victoryTurn() == PLAYER)
		return -10;

	// Else if none of them have won then return 0
	return 0;
}

int Game::minimax(Board board, int depth, bool isMax)
{
	int score = evaluate(board);

	// If Maximizer has won the game return his/her
	// evaluated score
	if (score == 10)
		return score;

	// If Minimizer has won the game return his/her
	// evaluated score
	if (score == -10)
		return score;

	// If there are no more moves and no winner then
	// it is a tie
	if (board.isTie())
		return 0;

	// If this maximizer's move
	if (isMax)
	{
		int best = -1000;

		// Traverse all cells
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				// Check if cell is empty
				if (board.m_grid[i][j] >= '1' && board.m_grid[i][j] <= '9')
				{
					char prev = board.m_grid[i][j];
					// Make the move
					board.m_grid[i][j] = OPPONENT;

					// Call minimax recursively and choose
					// the maximum value
					best = std::max(best, minimax(board, depth + 1, !isMax));

					// Undo the move
					board.m_grid[i][j] = prev;
				}
			}
		}
		return best;
	}

	// If this minimizer's move
	else
	{
		int best = 1000;

		// Traverse all cells
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				// Check if cell is empty
				if (board.m_grid[i][j] >= '1' && board.m_grid[i][j] <= '9')
				{
					char prev = board.m_grid[i][j];
					// Make the move
					board.m_grid[i][j] = PLAYER;

					// Call minimax recursively and choose
					// the minimum value
					best = std::min(best, minimax(board, depth + 1, !isMax));

					// Undo the move
					board.m_grid[i][j] = prev;
				}
			}
		}
		return best;
	}
}

// This will return the best possible move for the player
Move Game::findBestMove(Board board)
{
	int bestVal = -1000;
	Move bestMove;
	bestMove.row = -1;
	bestMove.col = -1;

	// Traverse all cells, evaluate minimax function for
	// all empty cells. And return the cell with optimal
	// value.
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Check if cell is empty
			if (board.m_grid[i][j] >= '1' && board.m_grid[i][j] <= '9')
			{
				char prev = board.m_grid[i][j];
				// Make the move
				board.m_grid[i][j] = OPPONENT;

				int moveVal = minimax(board, 0, false);

				// Undo the move
				board.m_grid[i][j] = prev;

				if (moveVal > bestVal)
				{
					bestMove.row = i;
					bestMove.col = j;
					bestVal = moveVal;
				}
			}
		}
	}

	return bestMove;
}