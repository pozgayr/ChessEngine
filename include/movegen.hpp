#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>
#include "board.hpp"
#include "move.hpp"

constexpr uint64_t rank2 = 0x000000000000FF00ULL;
constexpr uint64_t rank7 = 0x00FF000000000000ULL;

class MoveGenerator {
	private:
		void pawnMoves(const Board &board);
	public:
		std::vector<Move> moves;
		void genMoves(const Board &board);
		
};

#endif
