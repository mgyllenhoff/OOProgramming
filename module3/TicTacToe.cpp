#include <iostream>
#include "TicTacToe.h"

/**
 * @file TicTacToe.cpp
 * @brief Implementation of the TicTacToe game class.
 *
 * This file contains the function definitions for the TicTacToe class,
 * including drawing the board, handling user and computer turns,
 * and checking game status.
 */

/**
 * @brief Constructs a TicTacToe game.
 *
 * Initializes the board with empty spaces and seeds the random
 * number generator for computer moves.
 */
TicTacToe::TicTacToe()
{
    // Create an empty board, filled with spaces that will be
    // replaced with symbols
    this->board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));

    // Set the seed for the random number generator so the
    // computer’s moves will be different each game
    std::srand(static_cast<unsigned int>(std::time(0)));
}

/**
 * @brief Draws the current state of the TicTacToe board.
 *
 * Displays the board in a 3x3 grid with symbols and separators.
 */
void TicTacToe::drawBoard()
{
    std::cout << std::endl;
    for (int i = 0; i < 3; i++)
    {
        std::cout << " ";
        for (int j = 0; j < 3; j++)
        {
            std::cout << this->board[i][j];
            if (j < 2)
            {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
        if (i < 2)
        {
            std::cout << "---+---+---\n";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Handles the user's turn.
 *
 * Prompts the user for input, validates the move, and places an 'X'
 * on the board at the chosen position.
 */
void TicTacToe::usersTurn()
{
    int row, col;
    while (true)
    {
        std::cout << "Input the location to place your marker (row# column#): ";
        std::cin >> row >> col;
        std::cout << std::endl;

        // Check that an integer was entered
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input (must be two integers)! Try again."
                      << std::endl;
            continue;
        }

        // Check that input is in bounds of the board
        if (row < 1 || row > 3 || col < 1 || col > 3)
        {
            std::cout << "Invalid input (out of bounds)! Try again."
                      << std::endl;
            continue;
        }

        // Check that input has not already been marked
        if (this->board[row - 1][col - 1] != ' ')
        {
            std::cout << "Invalid input (spot already taken)! Try again."
                      << std::endl;
            continue;
        }

        this->board[row - 1][col - 1] = 'X';
        break;
    }
}

/**
 * @brief Handles the computer's turn.
 *
 * Finds available empty spots, chooses one at random, and places an 'O'
 * on the board.
 */
void TicTacToe::computersTurn()
{
    // Find empty spots the computer could play
    std::vector<std::pair<int, int>> emptySpots;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (this->board[i][j] == ' ')
            {
                emptySpots.push_back({i, j});
            }
        }
    }

    // Randomly choose from those spots
    if (!emptySpots.empty())
    {
        int idx = std::rand() % emptySpots.size();
        this->board[emptySpots[idx].first][emptySpots[idx].second] = 'O';
    }
}

/**
 * @brief Checks whether the game is over.
 *
 * Evaluates winning conditions for the given symbol (representing
 * the user or computer), and determines if the game is a tie.
 *
 * @param symbol The player's symbol ('X' for user, 'O' for computer).
 * @return true if the game has ended (win or tie), false otherwise.
 */
bool TicTacToe::checkGameOver(char symbol)
{
    // Check if someone has won (three in a row)
    for (int i = 0; i < 3; i++)
    {
        // Row by row
        if (this->board[i][0] == symbol && this->board[i][1] == symbol &&
            this->board[i][2] == symbol)
        {
            if (symbol == 'X')
            {
                std::cout << "You won!" << std::endl;
            }
            else
            {
                std::cout << "You lost!" << std::endl;
            }
            return true;
        }
        // Column by column
        if (this->board[0][i] == symbol && this->board[1][i] == symbol &&
            this->board[2][i] == symbol)
        {
            if (symbol == 'X')
            {
                std::cout << "You won!" << std::endl;
            }
            else
            {
                std::cout << "You lost!" << std::endl;
            }
            return true;
        }
    }
    // Diagonals
    if (this->board[0][0] == symbol && this->board[1][1] == symbol &&
        this->board[2][2] == symbol)
    {
        if (symbol == 'X')
        {
            std::cout << "You won!" << std::endl;
        }
        else
        {
            std::cout << "You lost!" << std::endl;
        }
        return true;
    }
    if (this->board[0][2] == symbol && this->board[1][1] == symbol &&
        this->board[2][0] == symbol)
    {
        if (symbol == 'X')
        {
            std::cout << "You won!" << std::endl;
        }
        else
        {
            std::cout << "You lost to the computer!" << std::endl;
        }
        return true;
    }

    // Check that the board is not full
    int emptySpots = 9;
    for (auto &row : this->board)
    {
        for (char cell : row)
        {
            if (cell != ' ')
            {
                emptySpots--;
            }
        }
    }
    if (emptySpots == 0)
    {
        std::cout << "You tied!" << std::endl;
        return true;
    }

    return false;
}

/**
 * @brief Starts the TicTacToe game loop.
 *
 * Alternates between the user and the computer until the game ends.
 */
void TicTacToe::beginGame()
{
    drawBoard();

    while (true)
    {
        usersTurn();
        std::cout << "YOUR TURN:" << std::endl;
        drawBoard();
        if (checkGameOver('X'))
        {
            break;
        }

        computersTurn();
        std::cout << "COMPUTERS TURN:" << std::endl;
        drawBoard();
        if (checkGameOver('O'))
        {
            break;
        }
    }
}