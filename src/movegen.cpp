#include "movegen.hpp"
#include <bit>

void MoveGenerator::genMoves(const Board &board) {
	moves.clear();
	pawnMoves(board);
}

void MoveGenerator::pawnMoves(const Board &board) {
	uint64_t white = board.bitboards[P];
	uint64_t black = board.bitboards[p];
	uint64_t empty = ~board.occupancies[all];

	uint64_t singlePushWhite = (white << size) & empty;

	while (singlePushWhite) {
		int to = __builtin_ctzll(singlePushWhite);
		int from = to - size;
		moves.push_back({from, to, P, 0});
		singlePushWhite &= singlePushWhite - 1;
	}

	uint64_t doublePushWhite = ((white & rank2) << size * 2) & empty & (empty << 8);

	while (doublePushWhite) {
		int to = __builtin_ctzll(doublePushWhite);
		int from = to - size * 2;
		moves.push_back({from, to, P, 0});
		doublePushWhite &= doublePushWhite - 1;
	}

	uint64_t singlePushBlack = (black << size) & empty;
	
	while (singlePushBlack) {
		int to = __builtin_ctzll(singlePushBlack);
		int from = to - size;
		moves.push_back({from, to, P, 0});
		singlePushBlack &= singlePushBlack - 1;
	}

	uint64_t doublePushBlack = ((black & rank2) << size * 2) & empty & (empty << 8);

	while (doublePushBlack) {
		int to = __builtin_ctzll(doublePushBlack);
		int from = to - size * 2;
		moves.push_back({from, to, P, 0});
		doublePushBlack &= doublePushBlack - 1;
		}
}
