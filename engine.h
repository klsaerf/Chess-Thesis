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
};


#endif //CHESS_THESIS_ENGINE_H