/* chess_thesis --- engine.h
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#ifndef CHESS_THESIS_ENGINE_H
#define CHESS_THESIS_ENGINE_H

#include "chessboard.h"

// Engine class
// Returns the best move for the given algorithm
class Engine {
public:
    Engine() = delete;

    // Minimax algorithm
    // Traverses all possible move orders, chooses the best move for both players.
    // Returns the maximizing move for its color.
    static std::pair<Move, int> minimax(const ChessBoard &chessBoard, int depth, Color color);

    // Parallelized minimax algorithm
    // Traverses all possible move orders, chooses the best move for both players.
    // Returns the maximizing move for its color.
    static std::pair<Move, int> parallelMinimax(const ChessBoard &chessBoard, int depth, Color color);

    // Alphabeta pruning algorithm
    // Tracks the local maximizing scores in alpha and beta parameters for white and black respectively.
    // If a move is worse than the local maximum, that branch is pruned and not searched further.
    static std::pair<Move, int> alphabeta(const ChessBoard &chessBoard, int depth, Color color);

    // Parallelized alphabeta pruning algorithm
    // Since parallelizing alphabeta algorithm isn't trivial, the first call is the same as parallelMinimax,
    // and the subsequent recursive calls are of alphabeta algorithm.
    static std::pair<Move, int> parallelAlphabeta(const ChessBoard &chessBoard, int depth, Color color);
private:
    // Private alphabeta algorithm
    // Since this algorithm takes additional parameters that shouldn't be messed with, putting the extra
    // parametered function into private field with the normal parameter sized overloaded function in the
    // public field is sensible.
    static std::pair<Move, int> alphabeta(const ChessBoard &chessBoard, int depth, Color color, int alpha, int beta);
};


#endif //CHESS_THESIS_ENGINE_H