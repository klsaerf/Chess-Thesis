/* chess_thesis --- board.cpp
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */

#include "board.h"
#include <iostream>

Board::Board(): whiteCanCastleShort_(true), whiteCanCastleLong_(true),
                blackCanCastleShort_(true), blackCanCastleLong_(true) {
    board_.reserve(BOARD_SIZE);

    // Initialize the board
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            // White pieces
            if (i == 0) {
                // Pieces
                if (j == 0 || j == BOARD_LENGTH - 1) {
                    board_.push_back('R');
                } else if (j == 1 || j == BOARD_LENGTH - 2) {
                    board_.push_back('N');
                } else if (j == 2 || j == BOARD_LENGTH - 3) {
                    board_.push_back('B');
                } else if (j == 3) {
                    board_.push_back('Q');
                } else if (j == 4) {
                    board_.push_back('K');
                }
            } else if (i == 1) {
                // Pawns
                board_.push_back('P');

            // Black pieces
            } else if (i == BOARD_LENGTH - 2) {
                // Pawns
                board_.push_back('p');
            } else if (i == BOARD_LENGTH - 1) {
                // Pieces
                if (j == 0 || j == BOARD_LENGTH - 1) {
                    board_.push_back('r');
                } else if (j == 1 || j == BOARD_LENGTH - 2) {
                    board_.push_back('n');
                } else if (j == 2 || j == BOARD_LENGTH - 3) {
                    board_.push_back('b');
                } else if (j == 3) {
                    board_.push_back('q');
                } else if (j == 4) {
                    board_.push_back('k');
                }
            }
            // Empty squares
            else {
                board_.push_back('0');
            }
        }
    }
}

void Board::printBoard() const {
    // Print the board from top down, row by row
    for (int i = BOARD_LENGTH - 1; i >= 0; i--) {
        // Print the ranks of the board
        std::cout << i + 1 << "  ";
        for (int j = 0; j < BOARD_LENGTH; j++) {
            std::cout << board_.at(i * BOARD_LENGTH + j) << ' ';
        }
        std::cout << std::endl;
    }

    // Print the files of the board
    std::cout << "   ";
    for (char i = 'a'; i <= 'h'; i++) {
        std::cout << i << ' ';
    }
}

bool Board::makeMove(const int begin, const int end) {
    // If the begin or end is less than 0, that means the coords inputted were faulty.
    // A piece cannot move if the beginning and the ending squares are the same
    if (begin < 0 || end < 0 || begin == end) {
        return false;
    }

    // An empty piece cannot move
    if (board_.at(begin) == '0') {
        return false;
    }

    // Swap the end piece for the beginning piece, and remove the beginning piece
    board_.at(end) = board_.at(begin);
    board_.at(begin) = '0';

    return true;
}

void Board::getAvailableMoves() {
}
