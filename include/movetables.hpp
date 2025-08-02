#include "board.hpp"

#ifndef MOVE_TABLES_HPP
#define MOVE_TABLES_HPP

class LookupTables {
	public:
		static uint64_t knightTable[size*size];
		static uint64_t kingTable[size*size];
		static unsigned int castlingRightsTable[size*size][size*size];
		static void computeAttackTables();
			
	private:
		static void computeKnightAttacks();
		static uint64_t knightAttacksFrom(int square);
		static void computeKingAttacks();
		static uint64_t kingAttacksFrom(int square);
		static void computeCastlingRights();
};

#endif
