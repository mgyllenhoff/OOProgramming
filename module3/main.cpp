#include <iostream>
#include "TicTacToe.h"

/**
 * @file main.cpp
 * @brief Entry point for the TicTacToe game.
 *
 * Creates a TicTacToe object and starts the game loop.
 *
 * @note To compile and run:
 * g++ main.cpp TicTacToe.cpp -o program && ./program
 */

/**
 * @brief Main function that runs the TicTacToe game.
 *
 * Instantiates a TicTacToe object and calls beginGame()
 * to start the user vs. computer game loop.
 *
 * @return int Returns 0 upon successful completion.
 */
int main()
{
    TicTacToe game;
    game.beginGame();

    return 0;
}