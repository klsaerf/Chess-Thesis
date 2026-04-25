/* ChessThesis --- testposition.cpp
 * Author: Kaan Pide
 * Date  : 04/04/2026
 */
#include "../src/chessboard.h"
#include "../src/engine.h"

#include <iostream>
#include <chrono>

// Macro to grab variable name
#define PRINT_MOVE(m, n) printMove(m, #n)

Move benchmarkEngine(const ChessBoard& board, const int depth, const Color engineColor,
                     std::pair<Move, int> (*engine)(const ChessBoard&, int, Color)) {
    // Start the timer for the engine's move
    const auto now = std::chrono::high_resolution_clock::now();

    const auto [move, _] = engine(board, depth, engineColor);
    // Measure the elapsed time and print it to console
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);

    std::cout << duration.count() << " ms to calculate" << std::endl;

    return move;
}

std::string indexToCoordinate(const int index) {
    // Adding the corresponding file or rank index to the char,
    // we get the file or rank in char format
    char rank = 'a' + (index % BOARD_LENGTH);
    char row = '1' + (index / BOARD_LENGTH);
    return {rank, row};
}

void printMove(Move move, const std::string& funcName) {
    std::cout << funcName << " played " << indexToCoordinate(move.first) << " "
    << indexToCoordinate(move.second) << std::endl;
}

int main(int argc, char* argv[]) {
    int depth;
    try {
        depth = std::stoi(argv[1]);
        if (depth <= 0) throw std::invalid_argument("Invalid depth");
    } catch (...) {
        std::cerr << "Please enter a valid depth number" << std::endl;
        return 1;
    }

    ChessBoard chessBoard;

    // Deep Blue vs. Kasparov, 1997 Game 2
    // 1-0 Deep Blue wins, this is move 37, white to play
    // Deep Blue played Be4 in this position
    // https://lichess.org/study/1tjS4Wyd/qTRBe8jk
    chessBoard.board = {
        'R', '0', '0', '0', '0', '0', 'K', '0',
        'R', '0', 'B', '0', '0', 'Q', 'P', '0',
        '0', '0', 'P', '0', '0', '0', '0', 'P',
        '0', 'P', 'p', '0', '0', '0', '0', '0',
        '0', 'p', '0', 'P', 'p', 'P', '0', '0',
        '0', '0', '0', 'b', '0', 'p', '0', 'p',
        '0', '0', '0', '0', '0', '0', 'p', '0',
        'r', '0', 'r', '0', 'q', '0', 'k', '0',
    };


    chessBoard.whiteCanCastleShort = false;
    chessBoard.whiteCanCastleLong = false;
    chessBoard.blackCanCastleShort = false;
    chessBoard.blackCanCastleLong = false;


    std::cout << "--- Beginning set position test ---" << std::endl;

    std::cout << "Deep Blue has played c2 e4" << std::endl;


    std::pair<Move, int>(*parallelMinimaxEngine)(const ChessBoard&, int, Color) = Engine::parallelMinimax;
    const Move minimaxMove = benchmarkEngine(chessBoard, depth, WHITE, parallelMinimaxEngine);
    PRINT_MOVE(minimaxMove, parallelMinimaxEngine);


    std::pair<Move, int>(*parallelAlphabetaEngine)(const ChessBoard&, int, Color) = Engine::parallelAlphabeta;
    const Move alphabetaMove = benchmarkEngine(chessBoard, depth, WHITE, parallelAlphabetaEngine);
    PRINT_MOVE(alphabetaMove, parallelAlphabetaEngine);

    return 0;
}