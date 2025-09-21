#include <vector>

/**
 * @file TicTacToe.h
 * @brief Declaration of the TicTacToe game class.
 *
 * Defines the TicTacToe class, its member variables,
 * and its public/private functions.
 */

/**
 * @class TicTacToe
 * @brief A simple TicTacToe game between a user and the computer.
 *
 * The TicTacToe class manages the 3x3 board, processes
 * user input, generates computer moves, checks win/tie
 * conditions, and runs the main game loop.
 */
class TicTacToe
{
public:
    /**
     * @brief Constructs a TicTacToe game.
     *
     * Initializes the 3x3 board with empty spaces and
     * seeds the random number generator for computer moves.
     */
    TicTacToe();

    /**
     * @brief Draws the current state of the board.
     *
     * Prints the 3x3 grid to the console with separators
     * and the placed symbols ('X' and 'O').
     */
    void drawBoard();

    /**
     * @brief Handles the user's turn.
     *
     * Prompts the user to select a position, validates the input,
     * and places an 'X' on the board.
     */
    void usersTurn();

    /**
     * @brief Handles the computer's turn.
     *
     * Selects a random empty spot on the board and places an 'O'.
     */
    void computersTurn();

    /**
     * @brief Checks whether the game is over.
     *
     * Evaluates rows, columns, and diagonals for three-in-a-row
     * wins and determines if the board is full (tie).
     *
     * @param symbol The player's symbol ('X' for user, 'O' for computer).
     * @return true if the game has ended (win or tie), false otherwise.
     */
    bool checkGameOver(char symbol);

    /**
     * @brief Starts the TicTacToe game loop.
     *
     * Alternates between user and computer turns until
     * a win or tie condition is reached.
     */
    void beginGame();

private:
    std::vector<std::vector<char>> board; ///< 3x3 board storing game state
};