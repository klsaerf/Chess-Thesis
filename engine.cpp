/* chess_thesis --- engine.cpp
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#include "engine.h"
#include "evaluator.h"
#include <algorithm>

std::pair<Move, int> Engine::minimax(const ChessBoard &chessBoard, const int depth, const Color color) {
    // If the game is over, return without checking deeper
    const Color gameOver = ChessBoardFunctions::isGameOver(chessBoard);
    if (gameOver != EMPTY) {
        // Add depth information to a winning move, so a faster checkmate is preferred
        const int winEval = gameOver == WHITE ? WHITE_WIN + depth : BLACK_WIN - depth;
        return {NO_MOVE, winEval};
    }
    // Base recursion step
    if (depth == 0) {
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

        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, BLACK).second};

            // If white's move is better than the current best, update it
            if (newBest.second > bestMove.second) bestMove = newBest;
        }

    } else if (color == BLACK) {
        // Initialize with worst move for black -> white win
        bestMove = {NO_MOVE, WHITE_WIN};

        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, WHITE).second};

            // If black's move is better than the current best, update it
            if (newBest.second < bestMove.second) bestMove = newBest;
        }
    }

    return bestMove;
}

std::pair<Move, int> Engine::parallelMinimax(const ChessBoard &chessBoard, int depth, Color color) {
    // If the game is over, return without checking deeper
    const Color gameOver = ChessBoardFunctions::isGameOver(chessBoard);
    if (gameOver != EMPTY) {
        // Add depth information to a winning move, so a faster checkmate is preferred
        const int winEval = gameOver == WHITE ? WHITE_WIN + depth : BLACK_WIN - depth;
        return {NO_MOVE, winEval};
    }
    // Base recursion step
    if (depth == 0) {
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

        // OpenMP parallelism
        // bestMove is shared among threads, while all the other variables are kept (first)private
        #pragma omp parallel for default(none) firstprivate(moves, depth, color, chessBoard) shared(bestMove)
        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, BLACK).second};

            // Critical section, avoiding race condition
            // Here, if two moves share the same eval, the first one to reach will update,
            // since the second one will fail the if check.
            // This makes it so in the early game, the engine plays a little more unpredictably.
            #pragma omp critical
            {
                // If white's move is better than the current best, update it
                if (newBest.second > bestMove.second) bestMove = newBest;
            }
        }

    } else if (color == BLACK) {
        // Initialize with worst move for black -> white win
        bestMove = {NO_MOVE, WHITE_WIN};

        // OpenMP parallelism
        // bestMove is shared among threads, while all the other variables are kept (first)private
        #pragma omp parallel for default(none) firstprivate(moves, depth, color, chessBoard) shared(bestMove)
        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, minimax(previous, depth - 1, WHITE).second};

            // Critical section, avoiding race condition
            // Here, if two moves share the same eval, the first one to reach will update,
            // since the second one will fail the if check.
            // This makes it so in the early game, the engine plays a little more unpredictably.
            #pragma omp critical
            {
                // If black's move is better than the current best, update it
                if (newBest.second < bestMove.second) bestMove = newBest;
            }
        }
    }

    return bestMove;
}

std::pair<Move, int> Engine::alphabeta(const ChessBoard &chessBoard, const int depth, const Color color) {
    // Encapsulation of the private parameters alpha and beta
    return alphabeta(chessBoard, depth, color, BLACK_WIN, WHITE_WIN);
}

std::pair<Move, int> Engine::parallelAlphabeta(const ChessBoard &chessBoard, int depth, Color color) {
    // If the game is over, return without checking deeper
    const Color gameOver = ChessBoardFunctions::isGameOver(chessBoard);
    if (gameOver != EMPTY) {
        // Add depth information to a winning move, so a faster checkmate is preferred
        const int winEval = gameOver == WHITE ? WHITE_WIN + depth : BLACK_WIN - depth;
        return {NO_MOVE, winEval};
    }
    // Base recursion step
    if (depth == 0) {
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

        // OpenMP parallelism
        // bestMove is shared among threads, while all the other variables are kept (first)private
        #pragma omp parallel for default(none) firstprivate(moves, depth, color, chessBoard) shared(bestMove)
        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, alphabeta(previous, depth - 1, BLACK).second};

            // Critical section, avoiding race condition
            // Here, if two moves share the same eval, the first one to reach will update,
            // since the second one will fail the if check.
            // This makes it so in the early game, the engine plays a little more unpredictably.
            #pragma omp critical
            {
                // If white's move is better than the current best, update it
                if (newBest.second > bestMove.second) bestMove = newBest;
            }
        }

    } else if (color == BLACK) {
        // Initialize with worst move for black -> white win
        bestMove = {NO_MOVE, WHITE_WIN};

        // OpenMP parallelism
        // bestMove is shared among threads, while all the other variables are kept (first)private
        #pragma omp parallel for default(none) firstprivate(moves, depth, color, chessBoard) shared(bestMove)
        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, alphabeta(previous, depth - 1, WHITE).second};

            // Critical section, avoiding race condition
            // Here, if two moves share the same eval, the first one to reach will update,
            // since the second one will fail the if check.
            // This makes it so in the early game, the engine plays a little more unpredictably.
            #pragma omp critical
            {
                // If black's move is better than the current best, update it
                if (newBest.second < bestMove.second) bestMove = newBest;
            }
        }
    }

    return bestMove;
}

std::pair<Move, int> Engine::alphabeta(const ChessBoard &chessBoard, const int depth, const Color color, int alpha, int beta) {
    // If the game is over, return without checking deeper
    const Color gameOver = ChessBoardFunctions::isGameOver(chessBoard);
    if (gameOver != EMPTY) {
        // Add depth information to a winning move, so a faster checkmate is preferred
        const int winEval = gameOver == WHITE ? WHITE_WIN + depth : BLACK_WIN - depth;
        return {NO_MOVE, winEval};
    }
    // Base recursion step
    if (depth == 0) {
        return {NO_MOVE, Evaluator::evaluate(chessBoard)};
    }

    // Tracks the best move for the current depth
    std::pair<Move, int> bestMove;

    // The moves to be made into the next depth
    std::vector<Move> moves;
    ChessBoardFunctions::getAvailableMoves(chessBoard, moves, color);

    // Shuffle the array so the better moves have a higher chance of being in front of the vector,
    // resulting in a speedup due to pruning more branches, especially on higher depths
    std::ranges::shuffle(moves, gen);

    // White color base
    if (color == WHITE) {
        // Initialize with worst move for white -> black win
        bestMove = {NO_MOVE, BLACK_WIN};

        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, alphabeta(previous, depth - 1, BLACK, alpha, beta).second};

            // If the found move is better than beta(black's best move), prune the branch
            if (newBest.second > beta) return newBest;

            // If the found move is better than alpha(white's best move), update the alpha
            if (newBest.second > alpha) alpha = newBest.second;

            // If white's move is better than the current best, update it
            if (newBest.second > bestMove.second) bestMove = newBest;

        }

    } else if (color == BLACK) {
        // Initialize with worst move for black -> white win
        bestMove = {NO_MOVE, WHITE_WIN};

        for (Move move : moves) {
            // Copy of the board as to not mess with other moves
            ChessBoard previous = chessBoard;
            ChessBoardFunctions::makeMove(previous, move, color);
            const std::pair<Move, int> newBest = {move, alphabeta(previous, depth - 1, WHITE, alpha, beta).second};

            // If the found move is better than alpha(white's best move), prune the branch
            if (newBest.second < alpha) return newBest;

            // If the found move is better than beta(black's best move), update the beta
            if (newBest.second < beta) beta = newBest.second;

            // If black's move is better than the current best, update it
            if (newBest.second < bestMove.second) bestMove = newBest;
        }
    }

    return bestMove;
}
