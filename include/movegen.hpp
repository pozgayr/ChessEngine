#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>
#include "board.hpp"
#include "move.hpp"


constexpr uint64_t rank1 = 0x00000000000000FFULL;
constexpr uint64_t rank2 = 0x000000000000FF00ULL;
constexpr uint64_t rank3 = 0x0000000000FF0000ULL;
constexpr uint64_t rank6 = 0x0000FF0000000000ULL;
constexpr uint64_t rank7 = 0x00FF000000000000ULL;
constexpr uint64_t rank8 = 0xFF00000000000000ULL;
constexpr uint64_t fileA = 0x0101010101010101ULL;
constexpr uint64_t fileH = 0x8080808080808080ULL;



class MoveGenerator {
	private:
		void pawnMoves(const Board &board, uint64_t pawns, Color side);
	public:
		std::vector<Move> moves;
		void genMoves(const Board &board);
		
};

#endif
