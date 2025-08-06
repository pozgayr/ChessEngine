#include "perft.hpp"

uint64_t perft(Board &board, int depth) {
	if (depth == 0) return 1ULL;

	MoveGenerator gen;
	gen.genMoves(board);

	uint64_t nodes = 0;
	for (auto &move : gen.moves) {
		board.makeMove(move);
		nodes += perft(board, depth-1);
		board.unmakeMove();
	}
	return nodes;
}

void perftDivide(Board &board, int depth) {
	MoveGenerator gen;
	gen.genMoves(board);

	uint64_t total = 0;
	for (auto &move : gen.moves) {
		board.makeMove(move);
		uint64_t nodes = perft(board, depth-1);
		std::cout << move.toString() << ": " << nodes << "\n";
		total += nodes;
		board.unmakeMove();
	}
	std::cout << "Total: " << total << "\n";
}
