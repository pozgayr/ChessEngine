#include "board.hpp"

#ifndef MOVE_TABLES_HPP
#define MOVE_TABLES_HPP

class LookupTables {
	public:
		static uint64_t knightTable[size * size];
		static uint64_t kingTable[size * size];
		static uint64_t rookTable[size * size];
		static uint64_t bishopTable[size * size];
		static uint64_t queenTable[size * size];
		static void computeAttackTables();
			
	private:
		static void computeKnightAttacks();
		static uint64_t knightAttacksFrom(int square);
		static void computeKingAttacks();
		static uint64_t kingAttacksFrom(int square);
		static void computeRookAttacks();
		static uint64_t rookAttacksFrom(int square);
		static void computeBishopAttacks();
		static uint64_t bishopAttacksFrom(int square);
		static void computeQueenAttacks();
		static uint64_t queenAttacksFrom(int square);
};

#endif
