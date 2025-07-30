#include "movegen.hpp"
#include <bit>
#include <array>

void MoveGenerator::genMoves(const Board &board) {
	moves.clear();

	if (board.side_to_move == WHITE) {
		pawnMoves(board, board.bitboards[P], WHITE);		
	}
	else {
		pawnMoves(board, board.bitboards[p], BLACK);		
	}
	
}

void MoveGenerator::pawnMoves(const Board &board, uint64_t pawns, Color side) {
	uint64_t empty = ~board.occupancies[all];
	uint64_t opponent = (side == WHITE) ? board.occupancies[black] : board.occupancies[white];

	int forward_shift = (side == WHITE) ? size : -size;
	int double_shift  = (side == WHITE) ? size*2 : -size*2;

	uint64_t promotion_rank = (side == WHITE) ? rank8 : rank1;
	uint64_t start_rank     = (side == WHITE) ? rank2 : rank7;

	uint64_t single_push = (side == WHITE) ? (pawns << size) & empty : (pawns >> size) & empty;
	 
	uint64_t promotions = single_push & promotion_rank;

	int pawn_piece = (side == WHITE) ? P : p;
	std::array<int, 4> promotion_pieces = (side == WHITE)
    	? std::array<int, 4>{Q, R, B, N}
	    : std::array<int, 4>{q, r, b, n};

	while (promotions) {
		int to = __builtin_ctzll(promotions);
		int from = to - forward_shift;
		for (int promo_piece : promotion_pieces) {
        	moves.push_back({from, to, pawn_piece, promo_piece, 0});
        }
        promotions &= promotions - 1;
	}

	uint64_t normal_push = single_push & ~promotion_rank;

	while (normal_push) {
		int to = __builtin_ctzll(normal_push);
		int from = to - forward_shift;
		moves.push_back({from, to, pawn_piece, 0, 0});
		normal_push &= normal_push - 1;
	}

	uint64_t double_push = ((pawns & start_rank) << (double_shift > 0 ? double_shift : 0)) & empty &
	                      ((side == WHITE) ? (empty << 8) : (empty >> 8));
	if (side == BLACK) double_push >>= (double_shift < 0 ? -double_shift : 0);

	while (double_push) {
		int to = __builtin_ctzll(double_push);
		int from = to - double_shift;
		moves.push_back({from, to, pawn_piece, 0, 0});
		double_push &= double_push - 1;
	}

	uint64_t left_capture = (side == WHITE) ? ((pawns << (size - 1)) & ~fileA & opponent)
											: ((pawns >> (size - 1)) & ~fileH & opponent);

	while (left_capture) {
		int to = __builtin_ctzll(left_capture);
		int from = (side == WHITE) ? to - (size - 1) : to + (size - 1);
		moves.push_back({from, to, pawn_piece, 0, 0});
		left_capture &= left_capture - 1;
	}

	uint64_t right_capture = (side == WHITE) ? ((pawns << (size + 1)) & ~fileH & opponent)
											 : ((pawns >> (size + 1)) & ~fileA & opponent);

	while (right_capture) {
		int to = __builtin_ctzll(right_capture);
		int from = (side == WHITE) ? to - (size + 1) : to + (size + 1);
		moves.push_back({from, to, pawn_piece, 0, 0});
		right_capture &= right_capture - 1;
	}
}
