/* chess_thesis --- main.cpp
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */

#include <iostream>
#include <vector>
#include <chrono>

#include "chessboard.h"
#include "engine.h"

// Splits a given string for the given delimiter.
// Returns the split words in a vector.
std::vector<std::string> split(const std::string& str, const char delimiter) {
    std::vector<std::string> result;
    std::string tmp = str;

    // Iterate through the string, cut it using .substr() from the delimiter position.
    // Push the word into the vector until the string is empty.
    while(tmp.find(delimiter) != std::string::npos) {
        std::string word = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter) + 1, tmp.size());
        if(!word.empty()) {
            result.push_back(word);
        }
    }
    // Add any leftover words
    if(!tmp.empty()) {
        result.push_back(tmp);
    }
    return result;
}

// Turns a given coordinate to index version.
// For example, coordinate "a1" would be index 0, and coordinate "e2" would be 12.
int coordinateToIndex(const std::string& coord) {
    // If a given coordinate isn't 2 characters long, return -1
    if (coord.length() != 2) {
        return -1;
    }

    // If the beginning of coordinate isn't within chess files, return -1
    if (tolower(coord[0]) < 'a' || tolower(coord[0]) > 'h') {
        return -1;
    }

    // If the ending of coordinate isn't within chess ranks, return -1
    if (coord[1] < '1' || coord[1] > '8') {
        return -1;
    }

    // Return the index of the coordinate
    return (tolower(coord[0]) - 'a') + (coord[1] - '1') * BOARD_LENGTH;
}

// Converts the given board index to board coordinates
// For example, index 0 would be "a1", and index 12 would be "e2".
std::string indexToCoordinate(const int index) {
    // Adding the corresponding file or rank index to the char,
    // we get the file or rank in char format
    char rank = 'a' + (index % BOARD_LENGTH);
    char row = '1' + (index / BOARD_LENGTH);
    return {rank, row};
}


// Main function of the program.
int main() {

    ChessBoard board;
    ChessBoardFunctions::initBoard(board);

    Color playerColor;
    Color engineColor;
    int depth;

    std::string input;

    // Parse the player's color
    while (true) {
        std::cout << "Enter the color to play (w/b): ";

        getline(std::cin, input);
        if (tolower(input[0]) == 'w') {
            playerColor = WHITE;
            engineColor = BLACK;
            break;
        }
        if (tolower(input[0]) == 'b') {
            playerColor = BLACK;
            engineColor = WHITE;
            break;
        }
    }

    // Parse the engine depth
    while (true) {
        std::cout << "Enter depth for engine: ";
        getline(std::cin, input);

        try {
            depth = std::stoi(input);
        } catch (const std::exception& e) {
            continue;
        }
        break;
    }

    // If the engine has white pieces, start with its move
    if (engineColor == WHITE) {
        // Start the timer for the engine's move
        auto now = std::chrono::high_resolution_clock::now();

        const std::pair<Move, int> engine = Engine::minimax(board, depth, engineColor);
        // Measure the elapsed time and print it to console
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);
        std::cout << duration.count() << " ms to think" << std::endl;

        // Make the engine's move and print it to console
        ChessBoardFunctions::makeMove(board, engine.first, engineColor);
        std::cout << "The bot played " << indexToCoordinate(engine.first.first)
        << " to " << indexToCoordinate(engine.first.second) << std::endl;
    }


    // Main loop
    while (true) {
        ChessBoardFunctions::printBoard(board);
        std::cout << std::endl;
        std::cout << "Enter move (q to exit): ";
        getline(std::cin, input);

        // Break the loop
        if (input == "q" || input == "Q") {
            break;
        }

        Move move;

        // Castling inputs
        if (input == "O-O") {
            move = playerColor == WHITE ? WHITE_CASTLE_SHORT : BLACK_CASTLE_SHORT;
        } else if (input == "0-0-0") {
            move = playerColor == WHITE ? WHITE_CASTLE_LONG : BLACK_CASTLE_LONG;
        } else {
            std::vector<std::string> coords = split(input, ' ');

            // Check if correct number of coordinates are inputted
            if (coords.size() != 2) {
                std::cout << "Please enter 2 coordinates." << std::endl;
                continue;
            }

            // Convert the coordinates to indices and attempt a move
            const int begin = coordinateToIndex(coords[0]);
            const int end = coordinateToIndex(coords[1]);

            move = {begin, end};
        }

        // Attempt to make a move, if not successful,
        if (const bool isMoveMade = ChessBoardFunctions::makeMove(board, move, playerColor); !isMoveMade) {
            std::cout << "No move could be made" << std::endl;
            continue;
        }

        // Start the timer for the engine's move
        auto now = std::chrono::high_resolution_clock::now();

        const std::pair<Move, int> engine = Engine::minimax(board, depth, engineColor);
        // Measure the elapsed time and print it to console
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - now);
        std::cout << duration.count() << " ms to think" << std::endl;

        // Make the engine's move and print it to console
        ChessBoardFunctions::makeMove(board, engine.first, engineColor);
        std::cout << "The bot played " << indexToCoordinate(engine.first.first)
        << " to " << indexToCoordinate(engine.first.second) << std::endl;

    }

    return 0;
}