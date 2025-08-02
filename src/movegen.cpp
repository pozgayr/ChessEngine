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
	bishopMoves(board);
	queenMoves(board);
	pawnAttackMoves(board);
}

void MoveGenerator::castlingMoves(const Board &board) {
	
}

void MoveGenerator::queenMoves(const Board &board) {
	Color side = board.side_to_move;
	int queen_piece = (side == WHITE) ? Q : q;
	uint64_t queens = board.bitboards[queen_piece];
	int directions[][2] = {
		{0, 1}, {1, 0}, {0, -1}, {-1, 0},
		{1, 1}, {1, -1}, {-1, -1}, {-1, 1}
	};

	while (queens) {
		int from = __builtin_ctzll(queens);
		traverseDirection(from, directions, 8, queen_piece, board);
		queens &= queens - 1;
	}
}

void MoveGenerator::rookMoves(const Board &board) {
	Color side = board.side_to_move;
	int rook_piece = (side == WHITE) ? R : r;
	uint64_t rooks = board.bitboards[rook_piece];
	int directions[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
	
	while (rooks) {
		int from = __builtin_ctzll(rooks);
		traverseDirection(from, directions, 4, rook_piece, board);
		rooks &= rooks - 1;
	}	
}

void MoveGenerator::bishopMoves(const Board &board) {
	Color side = board.side_to_move;
	int bishop_piece = (side == WHITE) ? B : b;
	uint64_t bishops = board.bitboards[bishop_piece];
	int directions[][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

	while (bishops) {
		int from = __builtin_ctzll(bishops);
		traverseDirection(from, directions, 4, bishop_piece, board);
		bishops &= bishops - 1;
	}
}

void MoveGenerator::traverseDirection(int from, const int directions[][2], int count, int piece, const Board &board) {
	Color side = board.side_to_move;
	uint64_t ally = board.occupancies[side];
	uint64_t enemy = board.occupancies[!side];

	for (int i = 0; i < count; i++) {
		int r = from / size;
		int f = from % size;
		
		int dr = directions[i][0];
		int df = directions[i][1];

		while (true) {
			r += dr;
			f += df;
	
			if (r < 0 || r >= size || f < 0 || f >= size) break;
			int square = r * size + f;

			if (ally & (1ULL << square)) break;
			moves.push_back({from, square, piece});
			if (enemy & (1ULL << square)) break;
		}
	}
}

void MoveGenerator::kingMoves(const Board &board) {
	Color side = board.side_to_move;
	int king_piece = (side == WHITE) ? K : k;	
	uint64_t king = board.bitboards[king_piece];
	uint64_t ally_pieces = board.occupancies[side];

	while (king) {
		int from = __builtin_ctzll(king);
		uint64_t attacks = LookupTables::kingTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			moves.push_back({from, to, king_piece});
			attacks &= attacks - 1;
		}
		king &= king - 1;
	}
}

void MoveGenerator::knightMoves(const Board &board) {
	Color side = board.side_to_move;
	int knight_piece = (side == WHITE) ? N : n;
	uint64_t knights = board.bitboards[knight_piece];
	uint64_t ally_pieces = board.occupancies[side];
	
	
	while (knights) {
		int from = __builtin_ctzll(knights);
		uint64_t attacks = LookupTables::knightTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			moves.push_back({from, to, knight_piece});
			attacks &= attacks - 1;
		}
		knights &= knights - 1;
	}
}

void MoveGenerator::pawnAttackMoves(const Board &board) {
	Color side = board.side_to_move;
	int pawn_piece = (side == WHITE) ? P : p;
	uint64_t pawns = board.bitboards[pawn_piece];
	uint64_t opponent = board.occupancies[!side];

	uint64_t left_capture = (side == WHITE) ? ((pawns << (size - 1)) & ~fileA & opponent)
												: ((pawns >> (size - 1)) & ~fileH & opponent);
	
	while (left_capture) {
		int to = __builtin_ctzll(left_capture);
		int from = (side == WHITE) ? to - (size - 1) : to + (size - 1);
		moves.push_back({from, to, pawn_piece});
		left_capture &= left_capture - 1;
	}

	uint64_t right_capture = (side == WHITE) ? ((pawns << (size + 1)) & ~fileH & opponent)
											 : ((pawns >> (size + 1)) & ~fileA & opponent);

	while (right_capture) {
		int to = __builtin_ctzll(right_capture);
		int from = (side == WHITE) ? to - (size + 1) : to + (size + 1);
		moves.push_back({from, to, pawn_piece});
		right_capture &= right_capture - 1;
	}

	uint64_t enpassant_left = (side == WHITE) ? ((pawns << (size - 1)) & ~fileA & board.enpassant)
											  : ((pawns >> (size - 1)) & ~fileH & board.enpassant);
											  
	if (enpassant_left) {
		int to = __builtin_ctzll(enpassant_left);
		int from = (side == WHITE) ? to - (size - 1) : to + (size - 1);
		Move m{from, to, pawn_piece};
		m.enpassant = true;
		moves.push_back(m);
	}

	uint64_t enpassant_right = (side == WHITE) ? ((pawns << (size + 1)) & ~fileH & board.enpassant)
											   : ((pawns >> (size + 1)) & ~fileA & board.enpassant);

	if (enpassant_right) {
		int to = __builtin_ctzll(enpassant_right);
		int from = (side == WHITE) ? to - (size + 1) : to + (size + 1);
		Move m{from, to, pawn_piece};
		m.enpassant = true;
		moves.push_back(m);
	}
}

void MoveGenerator::pawnMoves(const Board &board) {
	Color side = board.side_to_move;
	int pawn_piece = (side == WHITE) ? P : p;
	uint64_t pawns = board.bitboards[pawn_piece];
	uint64_t empty = ~board.occupancies[all];
	
	int forward_shift = (side == WHITE) ? size : -size;
	int double_shift  = (side == WHITE) ? size*2 : -size*2;

	uint64_t promotion_rank = (side == WHITE) ? rank8 : rank1;
	uint64_t start_rank     = (side == WHITE) ? rank2 : rank7;

	uint64_t single_push = (side == WHITE) ? (pawns << size) & empty : (pawns >> size) & empty;
	 
	uint64_t promotions = single_push & promotion_rank;

	
	std::array<int, 4> promotion_pieces = (side == WHITE)
    	? std::array<int, 4>{Q, R, B, N}
	    : std::array<int, 4>{q, r, b, n};

	while (promotions) {
		int to = __builtin_ctzll(promotions);
		int from = to - forward_shift;
		for (int promo_piece : promotion_pieces) {
			Move m{from, to, pawn_piece};
			m.promotion = promo_piece;
        	moves.push_back(m);
        }
        promotions &= promotions - 1;
	}

	uint64_t normal_push = single_push & ~promotion_rank;

	while (normal_push) {
		int to = __builtin_ctzll(normal_push);
		int from = to - forward_shift;
		moves.push_back({from, to, pawn_piece});
		normal_push &= normal_push - 1;
	}

	uint64_t double_push = (side == WHITE) ? (((pawns & start_rank) << (size * 2)) & empty 
						 & (empty << 8)) : (((pawns & start_rank) 
						 >> (size * 2)) & empty & (empty >> 8));   

	while (double_push) {
		int to = __builtin_ctzll(double_push);
		int from = to - double_shift;
		moves.push_back({from, to, pawn_piece});
		double_push &= double_push - 1;
	}
}
