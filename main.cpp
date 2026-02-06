/* chess_thesis --- main.cpp
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */

#include <iostream>
#include <vector>
#include "chessboard.h"
#include "evaluator.h"

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

// Main function of the program.
int main() {

    ChessBoard board;
    ChessBoardFunctions::initBoard(board);

    std::string input;

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

        std::vector<std::string> coords = split(input, ' ');

        // Check if correct number of coordinates are inputted
        if (coords.size() != 2) {
            std::cout << "Please enter 2 coordinates." << std::endl;
            continue;
        }

        // Convert the coordinates to indices and attempt a move
        const int begin = coordinateToIndex(coords[0]);
        const int end = coordinateToIndex(coords[1]);

        const Move move(begin, end);

        bool isMoveMade = ChessBoardFunctions::makeMove(board, move);

        if (!isMoveMade) {
            std::cout << "No move could be made" << std::endl;
        }
    }

    return 0;
}