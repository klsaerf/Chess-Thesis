/* chess_thesis --- board.cpp
 * Author: Kaan Pide
 * Date  : 28/01/2026
 */

#include "chessboard.h"
#include <iostream>


bool isInBounds(const int x, const int y) {
    return x >= 0 && x < BOARD_LENGTH && y >= 0 && y < BOARD_LENGTH;
}

ChessBoard::ChessBoard(): whiteCanCastleShort(true), whiteCanCastleLong(true),
                blackCanCastleShort(true), blackCanCastleLong(true) {
    board.reserve(BOARD_SIZE);
}

void ChessBoardFunctions::initBoard(ChessBoard &chessBoard) {
    // Initialize the board
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            // White pieces
            if (i == 0) {
                // Pieces
                if (j == 0 || j == BOARD_LENGTH - 1) {
                    chessBoard.board.push_back('R');
                } else if (j == 1 || j == BOARD_LENGTH - 2) {
                    chessBoard.board.push_back('N');
                } else if (j == 2 || j == BOARD_LENGTH - 3) {
                    chessBoard.board.push_back('B');
                } else if (j == 3) {
                    chessBoard.board.push_back('Q');
                } else if (j == 4) {
                    chessBoard.board.push_back('K');
                }
            } else if (i == 1) {
                // Pawns
                chessBoard.board.push_back('P');

                // Black pieces
            } else if (i == BOARD_LENGTH - 2) {
                // Pawns
                chessBoard.board.push_back('p');
            } else if (i == BOARD_LENGTH - 1) {
                // Pieces
                if (j == 0 || j == BOARD_LENGTH - 1) {
                    chessBoard.board.push_back('r');
                } else if (j == 1 || j == BOARD_LENGTH - 2) {
                    chessBoard.board.push_back('n');
                } else if (j == 2 || j == BOARD_LENGTH - 3) {
                    chessBoard.board.push_back('b');
                } else if (j == 3) {
                    chessBoard.board.push_back('q');
                } else if (j == 4) {
                    chessBoard.board.push_back('k');
                }
            }
            // Empty squares
            else {
                chessBoard.board.push_back('0');
            }
        }
    }
}

void ChessBoardFunctions::printBoard(const ChessBoard& chessBoard) {
    // Print the board from top down, row by row
    for (int i = BOARD_LENGTH - 1; i >= 0; i--) {
        // Print the ranks of the board
        std::cout << i + 1 << "  ";
        for (int j = 0; j < BOARD_LENGTH; j++) {
            std::cout << chessBoard.board.at(i * BOARD_LENGTH + j) << ' ';
        }
        std::cout << std::endl;
    }

    // Print the files of the board
    std::cout << "   ";
    for (char i = 'a'; i <= 'h'; i++) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

bool ChessBoardFunctions::makeMove(ChessBoard& chessBoard, const Move& move, const Color turn) {
    const int begin = move.first;
    const int end = move.second;
    // If the begin or end is less than 0, that means the coords inputted were faulty.
    // A piece cannot move if the beginning and the ending squares are the same
    if (begin < 0 || end < 0 || begin == end) {
        return false;
    }

    // A piece of different color of turn cannot move
    if (const Color color = getColor(chessBoard.board.at(begin)); color != turn) {
        return false;
    }

    // Get the possible set of moves from the beginning square
    std::vector<Move> moves;
    switch (toupper(chessBoard.board.at(begin))) {
        case 'P':
            getPawnMoves(chessBoard, moves, begin);
            break;
        case 'N':
            getKnightMoves(chessBoard, moves, begin);
            break;
        case 'B':
            getBishopMoves(chessBoard, moves, begin);
            break;
        case 'R':
            getRookMoves(chessBoard, moves, begin);
            break;
        case 'Q':
            getQueenMoves(chessBoard, moves, begin);
            break;
        case 'K':
            getKingMoves(chessBoard, moves, begin);
            break;
        default:
            break;
    }

    // If the piece cannot move there, return false
    if (std::ranges::find(moves, move) == moves.end()) {
        return false;
    }

    if (attemptCastling(chessBoard, move)) return true;

    invalidateCastling(chessBoard, move);

    // Swap the end piece for the beginning piece, and remove the beginning piece
    chessBoard.board.at(end) = chessBoard.board.at(begin);
    chessBoard.board.at(begin) = '0';

    // Promotion to queen
    if (end / BOARD_LENGTH == BOARD_LENGTH - 1 && chessBoard.board.at(end) == 'P') {
        chessBoard.board.at(end) = 'Q';
    }
    if (end / BOARD_LENGTH == 0 && chessBoard.board.at(end) == 'p') {
        chessBoard.board.at(end) = 'q';
    }

    return true;
}

void ChessBoardFunctions::getAvailableMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const Color color) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        const char piece = chessBoard.board.at(i);
        if (getColor(piece) != color) continue;

        switch (toupper(piece)) {
            case 'P':
                getPawnMoves(chessBoard, moves, i);
                break;
            case 'N':
                getKnightMoves(chessBoard, moves, i);
                break;
            case 'B':
                getBishopMoves(chessBoard, moves, i);
                break;
            case 'R':
                getRookMoves(chessBoard, moves, i);
                break;
            case 'Q':
                getQueenMoves(chessBoard, moves, i);
                break;
            case 'K':
                getKingMoves(chessBoard, moves, i);
                break;
            default:
                break;
        }
    }
}

Color ChessBoardFunctions::isGameOver(const ChessBoard &chessBoard) {
    // If the white king is missing, black wins
    const auto foundWhiteKing =
        std::ranges::find(chessBoard.board, 'K');
    if (foundWhiteKing == chessBoard.board.end()) return BLACK;

    // Vice versa for black
    const auto foundBlackKing =
        std::ranges::find(chessBoard.board, 'k');
    if (foundBlackKing == chessBoard.board.end()) return WHITE;

    // Both kings present -> game continues
    return EMPTY;
}

void ChessBoardFunctions::getPawnMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const int index) {
    const Color color = getColor(chessBoard.board.at(index));
    const Color oppositeColor = color == WHITE ? BLACK : WHITE;
    // The direction of the pawn
    const int direction = color == WHITE ? 1 : -1;

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Check if front square is empty
    if (chessBoard.board.at(index + direction * BOARD_LENGTH) == '0') {
        moves.emplace_back(index, index + direction * BOARD_LENGTH);

        // Check if the pawn is on starting position and both front squares are empty
        if ((y == 1 && color == WHITE) || (y == BOARD_LENGTH - 2 && color == BLACK)) {
            if (chessBoard.board.at(index + direction * BOARD_LENGTH * 2) == '0') {
                moves.emplace_back(index, index + direction * BOARD_LENGTH * 2);
            }
        }
    }

    // Check to the upper left square of pawn for capture
    if (x > 0) {
        if (getColor(chessBoard.board.at(index + direction * BOARD_LENGTH - 1)) == oppositeColor) {
            moves.emplace_back(index, index + direction * BOARD_LENGTH - 1);
        }
    }

    // Check to the upper right square of pawn for capture
    if (x < BOARD_LENGTH - 1) {
        if (getColor(chessBoard.board.at(index + direction * BOARD_LENGTH + 1)) == oppositeColor) {
            moves.emplace_back(index, index + direction * BOARD_LENGTH + 1);
        }
    }
}

void ChessBoardFunctions::getKnightMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const int index) {
    const Color color = getColor(chessBoard.board.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Lambda function for knight move
    auto knightMove = [chessBoard, index, color](const int x, const int y, std::vector<Move>& moves) {
        if (isInBounds(x, y) && getColor(chessBoard.board.at(x + y * BOARD_LENGTH)) != color) {
            moves.emplace_back(index, x + y * BOARD_LENGTH);
        }
    };

    // Check for all 8 possible squares a knight can move
    knightMove(x + 1, y + 2, moves);
    knightMove(x - 1, y + 2, moves);
    knightMove(x + 2, y + 1, moves);
    knightMove(x - 2, y + 1, moves);
    knightMove(x + 2, y - 1, moves);
    knightMove(x - 2, y - 1, moves);
    knightMove(x + 1, y - 2, moves);
    knightMove(x - 1, y - 2, moves);
}

void ChessBoardFunctions::getBishopMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const int index) {
    const Color color = getColor(chessBoard.board.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Flags to stop looking for in a given direction
    bool ppFlag = true;
    bool pnFlag = true;
    bool npFlag = true;
    bool nnFlag = true;

    for (int i = 1; i < BOARD_LENGTH; i++) {
        if (ppFlag) canMove(chessBoard, index, x + i, y + i, color, moves, ppFlag);
        if (pnFlag) canMove(chessBoard, index, x + i, y - i, color, moves, pnFlag);
        if (npFlag) canMove(chessBoard, index, x - i, y + i, color, moves, npFlag);
        if (nnFlag) canMove(chessBoard, index, x - i, y - i, color, moves, nnFlag);
    }
}

void ChessBoardFunctions::getRookMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const int index) {
    const Color color = getColor(chessBoard.board.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Flags to stop looking for in a given direction
    bool pvFlag = true;
    bool nvFlag = true;
    bool phFlag = true;
    bool nhFlag = true;

    for (int i = 1; i < BOARD_LENGTH; i++) {
        if (pvFlag) canMove(chessBoard, index, x + i, y, color, moves, pvFlag);
        if (nvFlag) canMove(chessBoard, index, x - i, y, color, moves, nvFlag);
        if (phFlag) canMove(chessBoard, index, x, y + i, color, moves, phFlag);
        if (nhFlag) canMove(chessBoard, index, x, y - i, color, moves, nhFlag);
    }
}

void ChessBoardFunctions::getQueenMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const int index) {
    // Queen is rook and bishop combined
    getBishopMoves(chessBoard, moves, index);
    getRookMoves(chessBoard, moves, index);
}

void ChessBoardFunctions::getKingMoves(const ChessBoard& chessBoard, std::vector<Move>& moves, const int index) {
    const Color color = getColor(chessBoard.board.at(index));

    const int x = index % BOARD_LENGTH;
    const int y = index / BOARD_LENGTH;

    // Check for 3x3 area centered around the king.
    // The king's own square will always fail, since it'll be same color as itself.
    for (int i =-1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (isInBounds(x + i, y + j) && getColor(chessBoard.board.at(x + i + (y + j) * BOARD_LENGTH)) != color) {
                moves.emplace_back(index, x + i + (y + j) * BOARD_LENGTH);
            }
        }
    }

    // Check for castling moves
    canCastle(chessBoard, moves, index);
}

void ChessBoardFunctions::canMove(const ChessBoard& chessBoard, const int index, const int x, const int y, const Color color, std::vector<Move>& moves, bool& flag) {
    // If the current square is out of bounds, return false and set flag to false
    if (const bool inBounds = isInBounds(x, y); !inBounds) {
        flag = false;
        return;
    }

    const Color targetColor = getColor(chessBoard.board.at(x + y * BOARD_LENGTH));

    // If the target square is not of its color, it means either empty or opponent piece
    if (targetColor != color) moves.emplace_back(index, x + y * BOARD_LENGTH);

    // Update the flag so it only keeps looking only if the square is empty
    flag = targetColor == EMPTY;
}

void ChessBoardFunctions::canCastle(const ChessBoard& chessBoard, std::vector<Move> &moves, const int index) {
    // Check if the king can castle and if it's still on starting square
    if (chessBoard.whiteCanCastleShort && index == 4) {
        // If the squares between are empty and the last square has the rook
        if (chessBoard.board.at(5) == '0' && chessBoard.board.at(6) == '0' && chessBoard.board.at(7) == 'R') {
            moves.emplace_back(WHITE_CASTLE_SHORT);
        }
    }
    if (chessBoard.whiteCanCastleLong && index == 4) {
        if (chessBoard.board.at(3) == '0' && chessBoard.board.at(2) == '0' && chessBoard.board.at(1) == '0' && chessBoard.board.at(0) == 'R') {
            moves.emplace_back(WHITE_CASTLE_LONG);
        }
    }
    if (chessBoard.blackCanCastleShort && index == 60) {
        if (chessBoard.board.at(61) == '0' && chessBoard.board.at(62) == '0' && chessBoard.board.at(63) == 'r') {
            moves.emplace_back(BLACK_CASTLE_SHORT);
        }
    }
    if (chessBoard.blackCanCastleLong && index == 60) {
        if (chessBoard.board.at(59) == '0' && chessBoard.board.at(58) == '0' && chessBoard.board.at(57) == '0' && chessBoard.board.at(56) == 'r') {
            moves.emplace_back(BLACK_CASTLE_LONG);
        }
    }
}

bool ChessBoardFunctions::attemptCastling(ChessBoard& chessBoard, const Move &move) {
    // If the given move is castling, update the squares accordingly.
    if (move == WHITE_CASTLE_SHORT) {
        chessBoard.board.at(4) = '0';
        chessBoard.board.at(5) = 'R';
        chessBoard.board.at(6) = 'K';
        chessBoard.board.at(7) = '0';
        chessBoard.whiteCanCastleShort = false;
        chessBoard.whiteCanCastleLong = false;
        return true;
    }
    if (move == WHITE_CASTLE_LONG) {
        chessBoard.board.at(0) = '0';
        // index 1 will be '0' before and after castling, so no need to update it
        chessBoard.board.at(2) = 'K';
        chessBoard.board.at(3) = 'R';
        chessBoard.board.at(4) = '0';
        chessBoard.whiteCanCastleShort = false;
        chessBoard.whiteCanCastleLong = false;
        return true;
    }
    if (move == BLACK_CASTLE_SHORT) {
        chessBoard.board.at(60) = '0';
        chessBoard.board.at(61) = 'r';
        chessBoard.board.at(62) = 'k';
        chessBoard.board.at(63) = '0';
        chessBoard.blackCanCastleShort = false;
        chessBoard.blackCanCastleLong = false;
        return true;
    }
    if (move == BLACK_CASTLE_LONG) {
        chessBoard.board.at(56) = '0';
        // index 57 will be '0' before and after castling, so no need to update it
        chessBoard.board.at(58) = 'k';
        chessBoard.board.at(59) = 'r';
        chessBoard.board.at(60) = '0';
        chessBoard.blackCanCastleShort = false;
        chessBoard.blackCanCastleLong = false;
        return true;
    }
    return false;
}

void ChessBoardFunctions::invalidateCastling(ChessBoard& chessBoard, const Move &move) {
    // If all flags are false, no need to check further
    if (!(chessBoard.whiteCanCastleShort || chessBoard.whiteCanCastleLong ||
          chessBoard.blackCanCastleShort || chessBoard.blackCanCastleLong)) {
        return;
    }

    // If the piece moving is in its starting position, and it is either king or rook
    if (chessBoard.board.at(move.first) == 'K' && move.first == 4) {
        // A king move invalidates both castling rights
        chessBoard.whiteCanCastleShort = false;
        chessBoard.whiteCanCastleLong = false;
        return;
    }
    if (chessBoard.board.at(move.first) == 'k' && move.first == 60) {
        chessBoard.blackCanCastleShort = false;
        chessBoard.blackCanCastleLong = false;
        return;
    }
    if (chessBoard.board.at(move.first) == 'R' && move.first == 7) {
        // A rook move only invalidates its castling side rights
        chessBoard.whiteCanCastleShort = false;
        return;
    }
    if (chessBoard.board.at(move.first) == 'R' && move.first == 0) {
        chessBoard.whiteCanCastleLong = false;
        return;
    }
    if (chessBoard.board.at(move.first) == 'r' && move.first == 63) {
        chessBoard.blackCanCastleShort = false;
        return;
    }
    if (chessBoard.board.at(move.first) == 'R' && move.first == 56) {
        chessBoard.blackCanCastleLong = false;
        return;
    }
}
