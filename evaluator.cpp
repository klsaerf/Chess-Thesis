/* chess_thesis --- evaluator.cpp
 * Author: Kaan Pide
 * Date  : 29/01/2026
 */
#include "evaluator.h"
#include <cctype>

int Evaluator::evaluate(const ChessBoard &chessBoard) {
    int evaluation = 0;

    // Loop through all pieces, add to eval if white, subtract if black
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (chessBoard.board.at(i) == '0') {
            continue;
        }
        const int color = getColor(chessBoard.board.at(i)) == WHITE ? 1 : -1;
        switch (tolower(chessBoard.board.at(i))) {
            case 'p':
                evaluation += PAWN_WEIGHT * color;
                evaluation += (color == 1 ? WHITE_PAWN_CENTRALITY.at(i) : BLACK_PAWN_CENTRALITY.at(i)) * color;
                break;
            case 'n':
                evaluation += (KNIGHT_WEIGHT + PIECE_CENTRALITY.at(i)) * color;
                break;
            case 'b':
                evaluation += (BISHOP_WEIGHT + PIECE_CENTRALITY.at(i)) * color;
                break;
            case 'r':
                evaluation += (ROOK_WEIGHT + PIECE_CENTRALITY.at(i)) * color;
                break;
            case 'q':
                evaluation += (QUEEN_WEIGHT + PIECE_CENTRALITY.at(i)) * color;
                break;
            case 'k':
                evaluation += (KING_WEIGHT + KING_CENTRALITY.at(i)) * color;
                break;
            default:
                break;
        }
    }

    return evaluation;
}
