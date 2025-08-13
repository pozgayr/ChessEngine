#include "eval.hpp"


int eval(Board &board) {
	MoveGenerator movegen;
	int score = 0;
	
	for (int piece = 0; piece < bitboard_count; piece++) {
		uint64_t bb = board.bitboards[piece];
		const int value = piece_values[piece];
        const int sign  = (piece >= 6) ? -1 : 1;
		while (bb) {
			int square = __builtin_ctzll(bb);
			int square_bonus = pieceSquareTable(piece, square);
			score += sign * (value + square_bonus);
			bb &= bb - 1;
		}
	}

	if (movegen.kingInCheck(board, WHITE)) score -= CHECK_BONUS;
	if (movegen.kingInCheck(board, BLACK)) score += CHECK_BONUS;

	if (!board.move_stack.empty()) {
       const Move &lastMove = board.move_stack.back();
       if (lastMove.capture != NONE) {
           score += CAPTURE_BONUS[lastMove.capture];
       }
   	}
	return score;
}

int scoreMove(const Move &m) {
	int score = 0;
	if (m.promotion) {
		score += 1000 + piece_values[m.promotion];
	}
	if (m.capture != NONE) {
		score += 500 * piece_values[m.capture] - piece_values[m.piece];
	}
	return score;
}

int drawScore(Board &board, int eval) {
	if (eval > DRAW_MARGIN)  return -DRAW_CONTEMPT; 
    if (eval < -DRAW_MARGIN) return +DRAW_CONTEMPT; 
    return 0;
}

int pieceSquareTable(int piece, int square) {
	bool is_black = piece >= 6;
	int type = piece % 6;
	int idx = (is_black) ? mirror(square) : square;

	switch (type) {
		case 0: return pawn_table[idx];
		case 1: return knight_table[idx];
		case 2: return bishop_table[idx];
		case 3: return rook_table[idx];
		case 4: return queen_table[idx];
		case 5: return king_table[idx];
		default: return 0;
	}
}	

int mirror(int square) {
	return square ^ 56;
}
