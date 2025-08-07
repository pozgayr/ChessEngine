#include "eval.hpp"

int eval(Board &board) {
	int score = 0;
	
	for (int piece = 0; piece < bitboard_count; piece++) {
		uint64_t bb = board.bitboards[piece];
		while (bb) {
			score += piece_values[piece];
			bb &= bb - 1;
		}
	} 
	return score;
}
