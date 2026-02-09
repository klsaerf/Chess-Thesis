/* chess_thesis --- engine.cpp
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#include "engine.h"

std::pair<Move, int> Engine::minimax(const ChessBoard &chessBoard, const int depth, const Color color) {
    if (ChessBoardFunctions::isGameOver(chessBoard) != EMPTY || depth == 0) {
        return {NO_MOVE, Evaluator::evaluate(chessBoard)};
    }

    std::pair<Move, int> bestMove;
    std::vector<Move> moves;
    ChessBoardFunctions::getAvailableMoves(chessBoard, moves, color);

    if (color == WHITE) {
        bestMove = {NO_MOVE, BLACK_WIN};

        ChessBoard previous = chessBoard;


        for (Move move : moves) {
            ChessBoardFunctions::makeMove(previous, move);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, BLACK).second};

            if (newBest.second > bestMove.second) {
                bestMove = newBest;
            }

            previous = chessBoard;
        }

    } else if (color == BLACK) {
        bestMove = {NO_MOVE, WHITE_WIN};

        ChessBoard previous = chessBoard;


        for (Move move : moves) {
            ChessBoardFunctions::makeMove(previous, move);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, WHITE).second};

            if (newBest.second < bestMove.second) {
                bestMove = newBest;
            }

            previous = chessBoard;
        }
    }

    return bestMove;
}