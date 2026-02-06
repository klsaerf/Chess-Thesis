/* chess_thesis --- board.h
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */
#ifndef CHESS_THESIS_BOARD_H
#define CHESS_THESIS_BOARD_H

#include <cctype>
#include <vector>

// Aliases
using Move = std::pair<int, int>;
// using Board = std::vector<char>;

// Board constants
constexpr int BOARD_LENGTH = 8;
constexpr int BOARD_SIZE = BOARD_LENGTH * BOARD_LENGTH;

// Castling constants
constexpr Move WHITE_CASTLE_SHORT(4, BOARD_SIZE);
constexpr Move WHITE_CASTLE_LONG(4, BOARD_SIZE + 1);
constexpr Move BLACK_CASTLE_SHORT(60, BOARD_SIZE);
constexpr Move BLACK_CASTLE_LONG(60, BOARD_SIZE + 1);

// Color of the pieces
enum Color { WHITE, BLACK, EMPTY };

// Returns the color of the piece
inline Color getColor(const char piece) {
    if (piece == '0') return EMPTY;
    return (isupper(piece)) ? WHITE : BLACK;
}

// ChessBoard struct
// Reserves the memory for the board on initialization.
// Tracks the board and the castling states.
struct ChessBoard {
    ChessBoard();

    std::vector<char> board;

    bool whiteCanCastleShort;
    bool whiteCanCastleLong;

    bool blackCanCastleShort;
    bool blackCanCastleLong;

};

// ChessBoardFunctions static class
// Handles the chess board related functionality such as printing the board, making moves and more.
class ChessBoardFunctions {
public:
    ChessBoardFunctions() = delete;

    // Initializes the board
    // White pieces are represented with uppercase letters, and black pieces with lowercase.
    static void initBoard(ChessBoard& chessBoard);

    // Prints the board.
    // The board is printed top down (White pieces are at the bottom).
    // The coordinates for ranks and files are printed alongside the board.
    static void printBoard(const ChessBoard& chessBoard);

    // Attempts to make a move.
    // If it can make a move, makes the move and returns true.
    // Otherwise, does nothing and returns false.
    static bool makeMove(ChessBoard& chessBoard, const Move& move);

    // Gets all the available moves on the board.
    static void getAvailableMoves(const ChessBoard& chessBoard, std::vector<Move>& moves);

private:
    // -- Private functions for retrieving the possible moves of the given piece. --

    // Extend the move vector with the moves a pawn can make on square index
    static void getPawnMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Extend the move vector with the moves a knight can make on square index
    static void getKnightMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Extend the move vector with the moves a bishop can make on square index
    static void getBishopMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Extend the move vector with the moves a rook can make on square index
    static void getRookMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Extend the move vector with the moves a queen can make on square index
    static void getQueenMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Extend the move vector with the moves a king can make on square index
    static void getKingMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Helper function that updates the move vector for the given target coordinates.
    // If target is out of bounds or the target square isn't empty, the flag is set to false.
    // The move is only added if the target square is in bounds and the target square has a different
    // color than the index color.
    static void canMove(const ChessBoard& chessBoard, int index, int x, int y, Color color, std::vector<Move>& moves, bool& flag);

    // Adds castling moves to moves vector if the given king can castle either long or short
    static void canCastle(const ChessBoard& chessBoard, std::vector<Move>& moves, int index);

    // Attempts to castle, returns true if successful, false otherwise
    static bool attemptCastling(ChessBoard& chessBoard, const Move &move);

    // If king or rook makes a non-castle move, invalidates castling rights accordingly.
    static void invalidateCastling(ChessBoard& chessBoard, const Move& move);
};


#endif //CHESS_THESIS_BOARD_H