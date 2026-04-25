/* chess_thesis --- test.cpp
 * Author: Kaan Pide
 * Date  : 03/04/2026
 */
#include "../src/chessboard.h"
#include "../src/engine.h"

#include <iostream>
#include <chrono>

// Macro to grab variable name
#define PRINT_TIME(t, n) printTime(t, #n)

long long benchmarkEngine(const ChessBoard& board, const int depth, const Color engineColor,
                     std::pair<Move, int> (*engine)(const ChessBoard&, int, Color)) {
    // Start the timer for the engine's move
    const auto now = std::chrono::high_resolution_clock::now();

    engine(board, depth, engineColor);
    // Measure the elapsed time
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);

    return duration.count();
}

void printTime(const std::string& time, const std::string& funcName) {
    std::cout << time << " ms for " << funcName << std::endl;
}

double getMean(const std::vector<long long>& arr) {
    double sum = 0;

    for (const auto a: arr) {
        sum += static_cast<double>(a);
    }

    return sum / static_cast<double>(arr.size());
}

double getStdDev(const std::vector<long long>& arr, const double mean) {
    double sum = 0;

    for (const auto a: arr) {
        sum += std::pow((static_cast<double>(a) - mean), 2);
    }

    return std::sqrt(sum / static_cast<double>(arr.size()));
}

// Repeats the engine move for a specified amount of time and returns the average
std::string repeatTest(const ChessBoard& board, const int depth, const int repeat, const bool verbose,
    std::pair<Move, int> (*engine)(const ChessBoard&, int, Color)) {
    std::vector<long long> time;

    for (int i = 0; i < repeat; i++) {
        if (i % 5 == 0 && verbose) std::cout << i << "/" << repeat << " done" << std::endl;

        time.push_back(benchmarkEngine(board, depth, WHITE, engine));
    }
    if (verbose) std::cout << repeat << "/" << repeat << " done" << std::endl;

    double mean = getMean(time);
    double stdDev = getStdDev(time, mean);

    std::string timeStr = std::to_string(mean) + " mean " + std::to_string(stdDev) + " std dev";

    return timeStr;
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

    int repeat;
    try {
        repeat = std::stoi(argv[2]);
        if (repeat <= 0) throw std::invalid_argument("Repeat must be greater than zero");
    } catch (...) {
        std::cerr << "Please enter a valid repeat number" << std::endl;
        return 1;
    }

    const bool verbose = argv[3] != nullptr;


    ChessBoard board;
    ChessBoardFunctions::initBoard(board);

    std::cout << "--- Beginning repeated tests on all functions ---" << std::endl;

    std::pair<Move, int>(*minimaxEngine)(const ChessBoard&, int, Color) = Engine::minimax;
    const std::string minimaxTime = repeatTest(board, depth, repeat, verbose, minimaxEngine);
    PRINT_TIME(minimaxTime, minimaxEngine);

    std::pair<Move, int>(*parallelMinimaxEngine)(const ChessBoard&, int, Color) = Engine::parallelMinimax;
    const std::string pMinimaxTime = repeatTest(board, depth, repeat, verbose, parallelMinimaxEngine);
    PRINT_TIME(pMinimaxTime, parallelMinimaxEngine);

    std::pair<Move, int>(*alphabetaEngine)(const ChessBoard&, int, Color) = Engine::alphabeta;
    const std::string alphabetaTime = repeatTest(board, depth, repeat, verbose, alphabetaEngine);
    PRINT_TIME(alphabetaTime, alphabetaEngine);

    std::pair<Move, int>(*parallelAlphabetaEngine)(const ChessBoard&, int, Color) = Engine::parallelAlphabeta;
    const std::string pAlphabetaTime = repeatTest(board, depth, repeat, verbose, parallelAlphabetaEngine);
    PRINT_TIME(pAlphabetaTime, parallelAlphabetaEngine);

    return 0;
}