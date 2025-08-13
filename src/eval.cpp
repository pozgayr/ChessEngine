#include "eval.hpp"

int eval(Board &board) {
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
	return score;
}

int scoreMove(const Move &m) {
	if (m.capture != NONE) {
		return 100 * piece_values[m.capture] - piece_values[m.piece];
	}
	if (m.promotion) {
		return 1000 + piece_values[m.promotion];
	} 
	return 0;
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
