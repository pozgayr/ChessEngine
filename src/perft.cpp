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

void perftDebug(Board &board, int depth, std::string debug) {
    MoveGenerator gen;
    gen.genMoves(board);
    for (auto &move : gen.moves) {
        if (move.toString() == debug) { // track specific move
            board.makeMove(move);
            std::cout << "DEBUG after " << move.toString() << ":\n";
            std::cout << "enpassant: " << board.enpassant << "\n";
            std::cout << "castling: " << board.castling_rights << "\n";
            perftDivide(board, depth - 1); // check deeper moves
            board.unmakeMove();
        }
    }
}
