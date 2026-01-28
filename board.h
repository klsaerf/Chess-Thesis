/* chess_thesis --- board.h
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */
#ifndef CHESS_THESIS_BOARD_H
#define CHESS_THESIS_BOARD_H

#include <vector>

const int BOARD_SIZE = 64;
const int BOARD_LENGTH = 8;

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

    void makeMove();

    void getAvailableMoves();

private:
    std::vector<char> board_;

    bool whiteCanCastleShort_;
    bool whiteCanCastleLong_;

    bool blackCanCastleShort_;
    bool blackCanCastleLong_;
};


#endif //CHESS_THESIS_BOARD_H