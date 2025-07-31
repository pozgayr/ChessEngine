#include "movegen.hpp"
#include "movetables.hpp"
#include <bit>
#include <array>

void MoveGenerator::genMoves(const Board &board) {
	moves.clear();

	pawnMoves(board);
	knightMoves(board);
	kingMoves(board);
	rookMoves(board);
}

void MoveGenerator::rookMoves(const Board &board) {
	Color side = board.side_to_move;
	int rook_piece = (side == WHITE) ? R : r;
	uint64_t rooks = board.bitboards[rook_piece];
	uint64_t ally_pieces = board.occupancies[side];

	while (rooks) {
		int from = __builtin_ctzll(rooks);
		uint64_t attacks = LookupTables::rookTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			moves.push_back({from, to, rook_piece, 0, 0});
			attacks &= attacks - 1;
		}
		rooks &= rooks - 1;
	}	
}

void MoveGenerator::kingMoves(const Board &board) {
	Color side = board.side_to_move;	
	uint64_t king = (side == WHITE) ? board.bitboards[K] : board.bitboards[k];
	uint64_t ally_pieces = board.occupancies[side];
	int king_piece = (side == WHITE) ? K : k;

	while (king) {
		int from = __builtin_ctzll(king);
		uint64_t attacks = LookupTables::kingTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			moves.push_back({from, to, king_piece, 0, 0});
			attacks &= attacks - 1;
		}
		king &= king - 1;
	}
}

void MoveGenerator::knightMoves(const Board &board) {
	Color side = board.side_to_move;
	uint64_t knights = (side == WHITE) ? board.bitboards[N] : board.bitboards[n];
	uint64_t ally_pieces = board.occupancies[side];
	int knight_piece = (side == WHITE) ? N : n;
	
	while (knights) {
		int from = __builtin_ctzll(knights);
		uint64_t attacks = LookupTables::knightTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			moves.push_back({from, to, knight_piece, 0, 0});
			attacks &= attacks - 1;
		}
		knights &= knights - 1;
	}
}

void MoveGenerator::pawnMoves(const Board &board) {
	Color side = board.side_to_move;
	uint64_t pawns = (side == WHITE) ? board.bitboards[P] : board.bitboards[p];
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

	uint64_t double_push = (side == WHITE) ? (((pawns & start_rank) << (size * 2)) & empty 
						 & (empty << 8)) : (((pawns & start_rank) 
						 >> (size * 2)) & empty & (empty >> 8));   

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

	uint64_t enpassant_left = (side == WHITE) ? ((pawns << (size - 1)) & ~fileA & board.enpassant)
											  : ((pawns >> (size - 1)) & ~fileH & board.enpassant);
											  
	if (enpassant_left) {
		int to = __builtin_ctzll(enpassant_left);
		int from = (side == WHITE) ? to - (size - 1) : to + (size - 1);
		moves.push_back({from, to, pawn_piece, 0, 1});
	}

	uint64_t enpassant_right = (side == WHITE) ? ((pawns << (size + 1)) & ~fileH & board.enpassant)
											   : ((pawns >> (size + 1)) & ~fileA & board.enpassant);

	if (enpassant_right) {
		int to = __builtin_ctzll(enpassant_right);
		int from = (side == WHITE) ? to - (size + 1) : to + (size + 1);
		moves.push_back({from, to, pawn_piece, 0, 1});
	}
}
