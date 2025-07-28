#include "movegen.hpp"
#include <bit>

void MoveGenerator::pawnMoves(const Board &board) {
	moves.clear();

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
}
