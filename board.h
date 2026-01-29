/* chess_thesis --- board.h
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */
#ifndef CHESS_THESIS_BOARD_H
#define CHESS_THESIS_BOARD_H

#include <vector>

// Board constants
const int BOARD_LENGTH = 8;
const int BOARD_SIZE = BOARD_LENGTH * BOARD_LENGTH;

// Color of the pieces
enum Color { WHITE, BLACK, EMPTY };

// Board Class
// Handles the chess board related functionality such as printing the board, making moves and more.
class Board {
public:
    Board();
    virtual ~Board() = default;

    // Prints the board.
    // The board is printed top down (White pieces are at the bottom).
    // The coordinates for ranks and files are printed alongside the board.
    void printBoard() const;

    // Attempts to make a move.
    // If it can make a move, makes the move and returns true.
    // Otherwise, does nothing and returns false.
    bool makeMove(int begin, int end);

    void getAvailableMoves();

private:
    // The board structure
    // Contains the pieces as chars along one dimension for easier management.
    std::vector<char> board_;

    // Flags for castling for both sides and colors
    bool whiteCanCastleShort_;
    bool whiteCanCastleLong_;

    bool blackCanCastleShort_;
    bool blackCanCastleLong_;
};


#endif //CHESS_THESIS_BOARD_H