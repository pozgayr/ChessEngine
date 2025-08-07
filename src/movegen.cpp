 #include "movegen.hpp"
#include "movetables.hpp"
#include <bit>
#include <array>

GameState MoveGenerator::genMoves(Board &board) {
	moves.clear();
	Color side = board.side_to_move;
	moveList pseudo_legal;
	attack_mask = 0ULL;

	MoveGenContext ctx{board, &pseudo_legal, attack_mask, side};
	
	pawnMoves(board, pseudo_legal);
	knightMoves(ctx);
	kingMoves(ctx);
	rookMoves(ctx);
	bishopMoves(ctx);
	queenMoves(ctx);
	pawnAttackMoves(ctx);
	castlingMoves(ctx);

	for (auto &move : pseudo_legal) {
		if (isMoveLegal(move, board)) {
			moves.push_back(move);		
		}
	}

	GameState current_state = ONGOING;
	if (moves.empty() && kingInCheck(board, side)) {
		current_state = CHECKMATE;
	} else if(moves.empty()) {
		current_state = STALEMATE;
	}
	return current_state;
}

bool MoveGenerator::isMoveLegal(const Move &m, Board &board) {
	Color side = board.side_to_move;
	board.makeMove(m);
	bool legal = !kingInCheck(board, side);
	board.unmakeMove();

	return legal;
}

bool MoveGenerator::squareAttacked(const Board &board, Color side, const std::vector<int> &squares) {
	uint64_t all_squares = 0ULL;
	for (int sq : squares) {
		all_squares |= (1ULL << sq);
	}
    uint64_t attacks = 0ULL;
    MoveGenContext ctx{board, nullptr, attacks, side};

    auto attacked = [&](uint64_t attacks) {
        return (attacks & all_squares) != 0ULL;
    };

	pawnAttackMoves(ctx);
    if (attacked(attacks)) return true;
    knightMoves(ctx);
    if (attacked(attacks)) return true;
    bishopMoves(ctx);
    if (attacked(attacks)) return true;
    rookMoves(ctx);
    if (attacked(attacks)) return true;
    queenMoves(ctx);
    if (attacked(attacks)) return true;
    kingMoves(ctx);
    if (attacked(attacks)) return true;
	
    return false;
}

bool MoveGenerator::kingInCheck(const Board &board, Color side) {
	int king_piece = (side == WHITE) ? K : k;
	int king_pos = __builtin_ctzll(board.bitboards[king_piece]);

	Color enemy_side = static_cast<Color>(!side);

	std::vector<int> pos_vector = {king_pos};

	return squareAttacked(board, enemy_side, pos_vector);
}

void MoveGenerator::castlingMoves(MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
	Color enemy_side = static_cast<Color>(!side);
	
	if (side == WHITE) {
	    if ((board.castling_rights & WK) &&
	        !(board.occupancies[all] & ((1ULL << f1) | (1ULL << g1))) &&
	        !squareAttacked(board, enemy_side, {e1, f1, g1})) {
	        if (ctx.out_moves) {
	            Move m{e1, g1, K};
	            m.castling = WK;
	            ctx.out_moves->push_back(m);
	        }
	    }
	
	    if ((board.castling_rights & WQ) &&
	        !(board.occupancies[all] & ((1ULL << b1) | (1ULL << c1) | (1ULL << d1))) &&
	        !squareAttacked(board, enemy_side, {e1, d1, c1})) {
	        if (ctx.out_moves) {
	            Move m{e1, c1, K};
	            m.castling = WQ;
	            ctx.out_moves->push_back(m);
	        }
	    }
	} else {
	    if ((board.castling_rights & BK) &&
	        !(board.occupancies[all] & ((1ULL << f8) | (1ULL << g8))) &&
	        !squareAttacked(board, enemy_side, {e8, f8, g8})) {
	        if (ctx.out_moves) {
	            Move m{e8, g8, k};
	            m.castling = BK;
	            ctx.out_moves->push_back(m);
	        }
	    }
	
	    if ((board.castling_rights & BQ) &&
	        !(board.occupancies[all] & ((1ULL << b8) | (1ULL << c8) | (1ULL << d8))) &&
	        !squareAttacked(board, enemy_side, {e8, d8, c8})) {
	        if (ctx.out_moves) {
	            Move m{e8, c8, k};
	            m.castling = BQ;
	            ctx.out_moves->push_back(m);
	        }
	    }
	}	
}

void MoveGenerator::queenMoves(MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
	int queen_piece = (side == WHITE) ? Q : q;
	uint64_t queens = board.bitboards[queen_piece];

	while (queens) {
		int from = __builtin_ctzll(queens);
		traverseDirection(from, queenDirections, 8, queen_piece, ctx);
		queens &= queens - 1;
	}
}

void MoveGenerator::rookMoves(MoveGenContext &ctx)  {
	Color side = ctx.side;
	const Board &board = ctx.board;
	int rook_piece = (side == WHITE) ? R : r;
	uint64_t rooks = board.bitboards[rook_piece];
	
	while (rooks) {
		int from = __builtin_ctzll(rooks);
		traverseDirection(from, rookDirections, 4, rook_piece, ctx);
		rooks &= rooks - 1;
	}	
}

void MoveGenerator::bishopMoves(MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
	int bishop_piece = (side == WHITE) ? B : b;
	uint64_t bishops = board.bitboards[bishop_piece];

	while (bishops) {
		int from = __builtin_ctzll(bishops);
		traverseDirection(from, bishopDirections, 4, bishop_piece, ctx);
		bishops &= bishops - 1;
	}
}

void MoveGenerator::traverseDirection(int from, const int directions[][2], 
									  int count, int piece, MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
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

			ctx.attack_mask |= (1ULL << square);
			if (ally & (1ULL << square)) break;
			if (ctx.out_moves) ctx.out_moves->push_back({from, square, piece});
			if (enemy & (1ULL << square)) break;
		}
	}
}

void  MoveGenerator::kingMoves(MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
	int king_piece = (side == WHITE) ? K : k;	
	uint64_t king = board.bitboards[king_piece];
	uint64_t ally_pieces = board.occupancies[side];

	while (king) {
		int from = __builtin_ctzll(king);
		uint64_t attacks = LookupTables::kingTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			if (ctx.out_moves) ctx.out_moves->push_back({from, to, king_piece});
			ctx.attack_mask |= (1ULL << to);
			attacks &= attacks - 1;
		}
		king &= king - 1;
	}
}

void MoveGenerator::knightMoves(MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
	int knight_piece = (side == WHITE) ? N : n;
	uint64_t knights = board.bitboards[knight_piece];
	uint64_t ally_pieces = board.occupancies[side];
	
	
	while (knights) {
		int from = __builtin_ctzll(knights);
		uint64_t attacks = LookupTables::knightTable[from] & ~ally_pieces;

		while (attacks) {
			int to = __builtin_ctzll(attacks);
			if (ctx.out_moves) ctx.out_moves->push_back({from, to, knight_piece});
			ctx.attack_mask |= (1ULL << to);
			attacks &= attacks - 1;
		}
		knights &= knights - 1;
	}
}

void MoveGenerator::pawnAttackMoves(MoveGenContext &ctx) {
	Color side = ctx.side;
	const Board &board = ctx.board;
	int pawn_piece = (side == WHITE) ? P : p;
	uint64_t pawns = board.bitboards[pawn_piece];
	uint64_t opponent = board.occupancies[!side];

	uint64_t promotion_rank = (side == WHITE) ? rank8 : rank1;

	uint64_t left_capture = (side == WHITE) ? ((pawns & ~fileA) << (size - 1))
												: ((pawns & ~fileH)  >> (size - 1));

	uint64_t right_capture = (side == WHITE) ? ((pawns & ~fileH) << (size + 1))
											 :((pawns & ~fileA) >> (size + 1));

	std::array<int, 4> promotion_pieces = (side == WHITE)
    	? std::array<int, 4>{Q, R, B, N}
	    : std::array<int, 4>{q, r, b, n};
												
	
	while (left_capture) {
		int to = __builtin_ctzll(left_capture);
		int from = (side == WHITE) ? to - (size - 1) : to + (size - 1);
		if ((1ULL << to) & promotion_rank) {
			for (int promo_piece : promotion_pieces) {
				if (ctx.out_moves && ((1ULL << to) & opponent)) {
					Move m{from, to, pawn_piece};
					m.promotion = promo_piece;
					ctx.out_moves->push_back(m);
				}
			}
		} else if (ctx.out_moves && ((1ULL << to) & opponent))  ctx.out_moves->push_back({from, to, pawn_piece});
		ctx.attack_mask |= (1ULL << to);
		left_capture &= left_capture - 1;
	}

	while (right_capture) {
		int to = __builtin_ctzll(right_capture);
		int from = (side == WHITE) ? to - (size + 1) : to + (size + 1);
		if ((1ULL << to) & promotion_rank) {
			for (int promo_piece : promotion_pieces) {
				if (ctx.out_moves && ((1ULL << to) & opponent)) {
					Move m{from, to, pawn_piece};
					m.promotion = promo_piece;
					ctx.out_moves->push_back(m); 
				}
			}
		} else if (ctx.out_moves && ((1ULL << to) & opponent)) ctx.out_moves->push_back({from, to, pawn_piece});
		ctx.attack_mask |= (1ULL << to);
		right_capture &= right_capture - 1;
	}


	uint64_t enpassant_left = (side == WHITE) ? ((pawns & ~fileA) << (size - 1)) & board.enpassant
											  : ((pawns & ~fileA) >> (size + 1)) & board.enpassant;
											  
	if (enpassant_left) {
	    int to = __builtin_ctzll(enpassant_left);
	    int from = (side == WHITE) ? to - (size - 1) : to + (size + 1);
	    if (ctx.out_moves) {
	        Move m{from, to, pawn_piece};
	        m.enpassant = true;
	        ctx.out_moves->push_back(m);
	    }
	    ctx.attack_mask |= (1ULL << to);
	}

	uint64_t enpassant_right = (side == WHITE) ? ((pawns & ~fileH) << (size + 1)) & board.enpassant
											   : ((pawns & ~fileH) >> (size - 1)) & board.enpassant;

	 if (enpassant_right) {
		int to = __builtin_ctzll(enpassant_right);
		int from = (side == WHITE) ? to - (size + 1) : to + (size - 1);
		if (ctx.out_moves) {
			Move m{from, to, pawn_piece};
			m.enpassant = true;
			ctx.out_moves->push_back(m);	
		}
		ctx.attack_mask |= (1ULL << to);
	}
}

void MoveGenerator::pawnMoves(const Board &board, moveList &out_moves) {
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
        	out_moves.push_back(m);
        }
        promotions &= promotions - 1;
	}

	uint64_t normal_push = single_push & ~promotion_rank;

	while (normal_push) {
		int to = __builtin_ctzll(normal_push);
		int from = to - forward_shift;
		out_moves.push_back({from, to, pawn_piece});
		normal_push &= normal_push - 1;
	}

	uint64_t double_push = (side == WHITE) ? (((pawns & start_rank) << (size * 2)) & empty 
						 & (empty << 8)) : (((pawns & start_rank) 
						 >> (size * 2)) & empty & (empty >> 8));   

	while (double_push) {
		int to = __builtin_ctzll(double_push);
		int from = to - double_shift;
		out_moves.push_back({from, to, pawn_piece});
		double_push &= double_push - 1;
	}
}
