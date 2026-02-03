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

    const Move move(begin, end);

    // If the piece cannot move there, return false
    if (std::ranges::find(moves, move) == moves.end()) {
        return false;
    }

    if (attemptCastling(move)) return true;

    invalidateCastling(move);

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

void Board::getAvailableMoves() const {
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

    std::cout << std::endl;
    for (auto move: moves) {
        std::cout << move.first << ' ' << move.second << std::endl;
    }
    std::cout << moves.size() << std::endl;
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

    // Flags to stop looking for in a given direction
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

    // Flags to stop looking for in a given direction
    bool pvFlag = true;
    bool nvFlag = true;
    bool phFlag = true;
    bool nhFlag = true;

    for (int i = 1; i < BOARD_LENGTH; i++) {
        if (pvFlag) canMove(index, x + i, y, color, moves, pvFlag);
        if (nvFlag) canMove(index, x - i, y, color, moves, nvFlag);
        if (phFlag) canMove(index, x, y + i, color, moves, phFlag);
        if (nhFlag) canMove(index, x, y - i, color, moves, nhFlag);
    }
}

void Board::getQueenMoves(std::vector<Move>& moves, const int index) const {
    // Queen is rook and bishop combined
    getBishopMoves(moves, index);
    getRookMoves(moves, index);
}

void Board::getKingMoves(std::vector<Move>& moves, const int index) const {
    const Color color = getColor(board_.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Check for 3x3 area centered around the king.
    // The king's own square will always fail, since it'll be same color as itself.
    for (int i =-1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (isInBounds(x + i, y + j) && getColor(board_.at(x + i + (y + j) * BOARD_LENGTH)) != color) {
                moves.emplace_back(index, x + i + (y + j) * BOARD_LENGTH);
            }
        }
    }

    // Check for castling moves
    canCastle(moves, index);
}

void Board::canMove(const int index, const int x, const int y, const Color color, std::vector<Move>& moves, bool& flag) const {
    // If the current square is out of bounds, return false and set flag to false
    if (const bool inBounds = isInBounds(x, y); !inBounds) {
        flag = false;
        return;
    }

    const Color targetColor = getColor(board_.at(x + y * BOARD_LENGTH));

    // If the target square is not of its color, it means either empty or opponent piece
    if (targetColor != color) moves.emplace_back(index, x + y * BOARD_LENGTH);

    // Update the flag so it only keeps looking only if the square is empty
    flag = targetColor == EMPTY;
}

void Board::canCastle(std::vector<Move> &moves, const int index) const {
    // Check if the king can castle and if it's still on starting square
    if (whiteCanCastleShort_ && index == 4) {
        // If the squares between are empty and the last square has the rook
        if (board_.at(5) == '0' && board_.at(6) == '0' && board_.at(7) == 'R') {
            moves.emplace_back(WHITE_CASTLE_SHORT);
        }
    }
    if (whiteCanCastleLong_ && index == 4) {
        if (board_.at(3) == '0' && board_.at(2) == '0' && board_.at(1) == '0' && board_.at(0) == 'R') {
            moves.emplace_back(WHITE_CASTLE_LONG);
        }
    }
    if (blackCanCastleShort_ && index == 60) {
        if (board_.at(61) == '0' && board_.at(62) == '0' && board_.at(63) == 'r') {
            moves.emplace_back(BLACK_CASTLE_SHORT);
        }
    }
    if (blackCanCastleLong_ && index == 60) {
        if (board_.at(59) == '0' && board_.at(58) == '0' && board_.at(57) == '0' && board_.at(56) == 'r') {
            moves.emplace_back(BLACK_CASTLE_LONG);
        }
    }
}

bool Board::attemptCastling(const Move &move) {
    // If the given move is castling, update the squares accordingly.
    if (move == WHITE_CASTLE_SHORT) {
        board_.at(4) = '0';
        board_.at(5) = 'R';
        board_.at(6) = 'K';
        board_.at(7) = '0';
        whiteCanCastleShort_ = false;
        whiteCanCastleLong_ = false;
        return true;
    }
    if (move == WHITE_CASTLE_LONG) {
        board_.at(0) = '0';
        // index 1 will be '0' before and after castling, so no need to update it
        board_.at(2) = 'K';
        board_.at(3) = 'R';
        board_.at(4) = '0';
        whiteCanCastleShort_ = false;
        whiteCanCastleLong_ = false;
        return true;
    }
    if (move == BLACK_CASTLE_SHORT) {
        board_.at(60) = '0';
        board_.at(61) = 'r';
        board_.at(62) = 'k';
        board_.at(63) = '0';
        blackCanCastleShort_ = false;
        blackCanCastleLong_ = false;
        return true;
    }
    if (move == BLACK_CASTLE_LONG) {
        board_.at(56) = '0';
        // index 57 will be '0' before and after castling, so no need to update it
        board_.at(58) = 'k';
        board_.at(59) = 'r';
        board_.at(60) = '0';
        blackCanCastleShort_ = false;
        blackCanCastleLong_ = false;
        return true;
    }
    return false;
}

void Board::invalidateCastling(const Move &move) {
    // If all flags are false, no need to check further
    if (!(whiteCanCastleShort_ || whiteCanCastleLong_ || blackCanCastleShort_ || blackCanCastleLong_)) {
        return;
    }

    // If the piece moving is in its starting position, and it is either king or rook
    if (board_.at(move.first) == 'K' && move.first == 4) {
        // A king move invalidates both castling rights
        whiteCanCastleShort_ = false;
        whiteCanCastleLong_ = false;
        return;
    }
    if (board_.at(move.first) == 'k' && move.first == 60) {
        blackCanCastleShort_ = false;
        blackCanCastleLong_ = false;
        return;
    }
    if (board_.at(move.first) == 'R' && move.first == 7) {
        // A rook move only invalidates its castling side rights
        whiteCanCastleShort_ = false;
        return;
    }
    if (board_.at(move.first) == 'R' && move.first == 0) {
        whiteCanCastleLong_ = false;
        return;
    }
    if (board_.at(move.first) == 'r' && move.first == 63) {
        blackCanCastleShort_ = false;
        return;
    }
    if (board_.at(move.first) == 'R' && move.first == 56) {
        blackCanCastleLong_ = false;
        return;
    }
}
