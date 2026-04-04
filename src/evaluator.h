/* chess_thesis --- evaluator.h
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#ifndef CHESS_THESIS_EVALUATOR_H
#define CHESS_THESIS_EVALUATOR_H

#include "chessboard.h"

constexpr int WHITE_WIN = 10000;
constexpr int BLACK_WIN = -10000;

// Constants for pieces weights (points)
constexpr int PAWN_WEIGHT = 10;
constexpr int KNIGHT_WEIGHT = 30;
constexpr int BISHOP_WEIGHT = 30;
constexpr int ROOK_WEIGHT = 50;
constexpr int QUEEN_WEIGHT = 90;
constexpr int KING_WEIGHT = 150;

// Centrality measure for different pieces on the board
const std::vector<int> PIECE_CENTRALITY = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 2, 2, 2, 2, 1, 0,
    0, 1, 2, 3, 3, 2, 1, 0,
    0, 1, 2, 3, 3, 2, 1, 0,
    0, 1, 2, 2, 2, 2, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

const std::vector<int> KING_CENTRALITY = {
    2, 2, 3, 2, 2, 2, 3, 2,
    2, 1, 1, 1, 1, 1, 1, 2,
    1, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 1,
    2, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 3, 2, 2, 2, 3, 2,
};

const std::vector<int> WHITE_PAWN_CENTRALITY = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4,
};

const std::vector<int> BLACK_PAWN_CENTRALITY = {
    4, 4, 4, 4, 4, 4, 4, 4,
    3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0,

};

// Evaluator class
// Handles the evaluation of the board
class Evaluator {
public:
    Evaluator() = delete;

    // Evaluates the board.
    // The evaluation is measured as a metric of both available pieces on the board,
    // and the centrality of those pieces.
    // A positive eval means the board is in White's favor, and a negative eval means Black's favor.
    static int evaluate(const ChessBoard& chessBoard);
};


#endif //CHESS_THESIS_EVALUATOR_H