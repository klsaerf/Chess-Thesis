/* chess_thesis --- engine.cpp
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#include "engine.h"
#include "evaluator.h"

std::pair<Move, int> Engine::minimax(const ChessBoard &chessBoard, const int depth, const Color color) {
    // Base recursion step, also if the game is over, return without checking deeper
    if (ChessBoardFunctions::isGameOver(chessBoard) != EMPTY || depth == 0) {
        return {NO_MOVE, Evaluator::evaluate(chessBoard)};
    }

    // Tracks the best move for the current depth
    std::pair<Move, int> bestMove;

    // The moves to be made into the next depth
    std::vector<Move> moves;
    ChessBoardFunctions::getAvailableMoves(chessBoard, moves, color);

    // White color base
    if (color == WHITE) {
        // Initialize with worst move for white -> black win
        bestMove = {NO_MOVE, BLACK_WIN};

        // Track the previous board separately, as the makeMove function modifies chessBoard inplace
        // This allows for each move to not affect each other
        ChessBoard previous = chessBoard;

        for (Move move : moves) {
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, BLACK).second};

            // If white's move is better than the current best, update it
            if (newBest.second > bestMove.second) {
                bestMove = newBest;
            }

            // Restore previous chessBoard
            previous = chessBoard;
        }

    } else if (color == BLACK) {
        // Initialize with worst move for black -> white win
        bestMove = {NO_MOVE, WHITE_WIN};

        // Track the previous board separately, as the makeMove function modifies chessBoard inplace
        // This allows for each move to not affect each other
        ChessBoard previous = chessBoard;

        // Track the previous board separately, as the makeMove function modifies chessBoard inplace
        // This allows for each move to not affect each other
        for (Move move : moves) {
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, WHITE).second};

            // If black's move is better than the current best, update it
            if (newBest.second < bestMove.second) {
                bestMove = newBest;
            }

            // Restore previous chessBoard
            previous = chessBoard;
        }
    }

    return bestMove;
}