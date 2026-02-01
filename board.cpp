/* chess_thesis --- board.cpp
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */

#include "board.h"
#include <iostream>

// Returns the color of the piece
Color getColor(const char piece) {
    if (piece == '0') return EMPTY;
    return (isupper(piece)) ? WHITE : BLACK;
}

bool isInBounds(const int x, const int y) {
    return x >= 0 && x < BOARD_LENGTH && y >= 0 && y < BOARD_LENGTH;
}

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

    // Get the possible set of moves from the beginning square
    std::vector<Move> moves;
    switch (toupper(board_.at(begin))) {
        case 'P':
            getPawnMoves(moves, begin);
            break;
        case 'N':
            getKnightMoves(moves, begin);
            break;
        case 'B':
            getBishopMoves(moves, begin);
            break;
        case 'R':
            getRookMoves(moves, begin);
            break;
        case 'Q':
            getQueenMoves(moves, begin);
            break;
        case 'K':
            getKingMoves(moves, begin);
            break;
        default:
            break;
    }

    // If the piece cannot move there, return false
    if (std::ranges::find(moves, std::pair(begin, end)) == moves.end()) {
        return false;
    }

    // Swap the end piece for the beginning piece, and remove the beginning piece
    board_.at(end) = board_.at(begin);
    board_.at(begin) = '0';

    // Promotion to queen
    if (end / BOARD_LENGTH == BOARD_LENGTH - 1 && board_.at(end) == 'P') {
        board_.at(end) = 'Q';
    }
    if (end / BOARD_LENGTH == 0 && board_.at(end) == 'p') {
        board_.at(end) = 'q';
    }

    return true;
}

void Board::getAvailableMoves() {
    std::vector<Move> moves;

    for (int i = 0; i < BOARD_SIZE; i++) {
        switch (const char piece = board_.at(i); toupper(piece)) {
            case 'P':
                getPawnMoves(moves, i);
                break;
            case 'N':
                getKnightMoves(moves, i);
                break;
            case 'B':
                getBishopMoves(moves, i);
                break;
            case 'R':
                getRookMoves(moves, i);
                break;
            case 'Q':
                getQueenMoves(moves, i);
                break;
            case 'K':
                getKingMoves(moves, i);
                break;
            default:
                break;
        }
    }
}

void Board::getPawnMoves(std::vector<Move>& moves, const int index) const {
    const Color color = getColor(board_.at(index));
    const Color oppositeColor = color == WHITE ? BLACK : WHITE;
    // The direction of the pawn
    const int direction = color == WHITE ? 1 : -1;

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Check if front square is empty
    if (board_.at(index + direction * BOARD_LENGTH) == '0') {
        moves.emplace_back(index, index + direction * BOARD_LENGTH);

        // Check if the pawn is on starting position and both front squares are empty
        if ((y == 1 && color == WHITE) || (y == BOARD_LENGTH - 2 && color == BLACK)) {
            if (board_.at(index + direction * BOARD_LENGTH * 2) == '0') {
                moves.emplace_back(index, index + direction * BOARD_LENGTH * 2);
            }
        }
    }

    // Check to the upper left square of pawn for capture
    if (x > 0) {
        if (getColor(board_.at(index + direction * BOARD_LENGTH - 1)) == oppositeColor) {
            moves.emplace_back(index, index + direction * BOARD_LENGTH - 1);
        }
    }

    // Check to the upper right square of pawn for capture
    if (x < BOARD_LENGTH - 1) {
        if (getColor(board_.at(index + direction * BOARD_LENGTH + 1)) == oppositeColor) {
            moves.emplace_back(index, index + direction * BOARD_LENGTH + 1);
        }
    }
}

void Board::getKnightMoves(std::vector<Move>& moves, const int index) const {
    const Color color = getColor(board_.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Lambda function for knight move
    auto knightMove = [this](const int index, const int x, const int y, std::vector<Move>& moves, const Color color) {
        if (isInBounds(x, y) && getColor(board_.at(x + y * BOARD_LENGTH)) != color) {
            moves.emplace_back(index, x + y * BOARD_LENGTH);
        }
    };

    // Check for all 8 possible squares a knight can move
    knightMove(index, x + 1, y + 2, moves, color);
    knightMove(index, x - 1, y + 2, moves, color);
    knightMove(index, x + 2, y + 1, moves, color);
    knightMove(index, x - 2, y + 1, moves, color);
    knightMove(index, x + 2, y - 1, moves, color);
    knightMove(index, x - 2, y - 1, moves, color);
    knightMove(index, x + 1, y - 2, moves, color);
    knightMove(index, x - 1, y - 2, moves, color);
}

void Board::getBishopMoves(std::vector<Move>& moves, const int index) const {
    const Color color = getColor(board_.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    bool ppFlag = true;
    bool pnFlag = true;
    bool npFlag = true;
    bool nnFlag = true;

    for (int i = 1; i < BOARD_LENGTH; i++) {
        if (ppFlag) canMove(index, x + i, y + i, color, moves, ppFlag);
        if (pnFlag) canMove(index, x + i, y - i, color, moves, pnFlag);
        if (npFlag) canMove(index, x - i, y + i, color, moves, npFlag);
        if (nnFlag) canMove(index, x - i, y - i, color, moves, nnFlag);
    }
}

void Board::getRookMoves(std::vector<Move>& moves, const int index) const {
    const Color color = getColor(board_.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    bool pvFlag = true;
    bool nvFlag = true;
    bool phFlag = true;
    bool nhFlag = true;

    for (int i = 1; i < BOARD_LENGTH; i++) {
        if (pvFlag) canMove(index, x + i, y, color, moves, pvFlag);
        if (nvFlag) canMove(index, x + i, y, color, moves, nvFlag);
        if (phFlag) canMove(index, x, y + i, color, moves, phFlag);
        if (nhFlag) canMove(index, x, y - i, color, moves, nhFlag);
    }
}

void Board::getQueenMoves(std::vector<Move>& moves, const int index) const {
    getBishopMoves(moves, index);
    getRookMoves(moves, index);
}

void Board::getKingMoves(std::vector<Move>& moves, const int index) const {
    const Color color = getColor(board_.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    for (int i =-1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (isInBounds(x + i, y + j) && getColor(board_.at(x + i + (y + j) * BOARD_LENGTH)) != color) {
                moves.emplace_back(index, x + i + (y + j) * BOARD_LENGTH);
            }
        }
    }
}

void Board::canMove(const int index, const int x, const int y, const Color color, std::vector<Move>& moves, bool& flag) const {
    const bool inBounds = isInBounds(x, y);
    if (!inBounds) {
        flag = false;
        return;
    }

    const Color targetColor = getColor(board_.at(x + y * BOARD_LENGTH));

    if (targetColor != color) moves.emplace_back(index, x + y * BOARD_LENGTH);

    flag = targetColor == EMPTY;
}
