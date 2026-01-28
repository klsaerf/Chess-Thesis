/* chess_thesis --- board.cpp
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */

#include "board.h"
#include <iostream>

Board::Board(): whiteCanCastleShort_(true), whiteCanCastleLong_(true),
                blackCanCastleShort_(true), blackCanCastleLong_(true) {
    board_.reserve(BOARD_SIZE);

    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            if (i == 0) {
                if (j == 0 || j == 7) {
                    board_.push_back('R');
                } else if (j == 1 || j == 6) {
                    board_.push_back('N');
                } else if (j == 2 || j == 5) {
                    board_.push_back('B');
                } else if (j == 3) {
                    board_.push_back('Q');
                } else if (j == 4) {
                    board_.push_back('K');
                }
            } else if (i == 1) {
                board_.push_back('P');
            } else if (i == 6) {
                board_.push_back('p');
            } else if (i == 7) {
                if (j == 0 || j == 7) {
                    board_.push_back('r');
                } else if (j == 1 || j == 6) {
                    board_.push_back('n');
                } else if (j == 2 || j == 5) {
                    board_.push_back('b');
                } else if (j == 3) {
                    board_.push_back('q');
                } else if (j == 4) {
                    board_.push_back('k');
                }
            }
            else {
                board_.push_back('0');
            }
        }
    }
}

void Board::printBoard() const {
    // Print the board from top down, row by row
    for (int i = BOARD_LENGTH - 1; i >= 0; i--) {
        std::cout << i + 1 << "  ";
        for (int j = 0; j < BOARD_LENGTH; j++) {
            std::cout << board_.at(i * BOARD_LENGTH + j) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "   ";
    for (char i = 'a'; i <= 'h'; i++) {
        std::cout << i << ' ';
    }
}

void Board::makeMove() {
}

void Board::getAvailableMoves() {
}
