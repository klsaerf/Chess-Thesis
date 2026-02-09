/* chess_thesis --- engine.h
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#ifndef CHESS_THESIS_ENGINE_H
#define CHESS_THESIS_ENGINE_H

#include "chessboard.h"
#include "evaluator.h"

class Engine {
public:
    Engine() = delete;

    static std::pair<Move, int> minimax(const ChessBoard &chessBoard, int depth, Color color);
};


#endif //CHESS_THESIS_ENGINE_H