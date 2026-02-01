/* chess_thesis --- board.h
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */
#ifndef CHESS_THESIS_BOARD_H
#define CHESS_THESIS_BOARD_H

#include <vector>

using Move = std::pair<int, int>;

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

    // -- Flags for castling for both sides and colors. --

    bool whiteCanCastleShort_;
    bool whiteCanCastleLong_;

    bool blackCanCastleShort_;
    bool blackCanCastleLong_;

    // -- Private functions for retrieving the possible moves of the given piece. --

    // Extend the move vector with the moves a pawn can make on square index
    void getPawnMoves(std::vector<Move>& moves, int index) const;

    // Extend the move vector with the moves a knight can make on square index
    void getKnightMoves(std::vector<Move>& moves, int index) const;

    // Extend the move vector with the moves a bishop can make on square index
    void getBishopMoves(std::vector<Move>& moves, int index) const;

    // Extend the move vector with the moves a rook can make on square index
    void getRookMoves(std::vector<Move>& moves, int index) const;

    // Extend the move vector with the moves a queen can make on square index
    void getQueenMoves(std::vector<Move>& moves, int index) const;

    // Extend the move vector with the moves a king can make on square index
    void getKingMoves(std::vector<Move>& moves, int index) const;

    // Helper function that updates the move vector for the given target coordinates.
    // If target is out of bounds or the target square isn't empty, the flag is set to false.
    // The move is only added if the target square is in bounds and the target square has a different
    // color than the index color.
    void canMove(int index, int x, int y, Color color, std::vector<Move>& moves, bool& flag) const;
};


#endif //CHESS_THESIS_BOARD_H