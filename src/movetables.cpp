#include "movetables.hpp"

uint64_t LookupTables::knightTable[size*size];
uint64_t LookupTables::kingTable[size*size];

uint64_t LookupTables::knightAttacksFrom(int square) {
	uint64_t attacks = 0ULL;

	int rank = square / size;
	int file = square % size;

	int offsets[8][2] = { 
		{2, -1}, {2, 1}, {1, 2}, {-1, 2}, 
		{-2, 1}, {-2, -1}, {1, -2}, {-1, -2}
	};

	for (auto &o : offsets) {
		int r = rank + o[0];
		int f = file + o[1];

		if (r >= 0 && r < size && f >= 0 && f < size) {
			attacks |= (1ULL << (r * size + f));
		}
	}
	return attacks;
}

void LookupTables::computeKnightAttacks() {
	for (int i = 0; i < size*size; i++) {
		knightTable[i] = knightAttacksFrom(i);		
	}
}

uint64_t LookupTables::kingAttacksFrom(int square) {
	uint64_t attacks = 0ULL;

	int rank = square / size;
	int file = square % size;

	int offsets[8][2] = {
		{1, 1}, {1, 0}, {1, -1},
		{0, 1}, {0, -1},
		{-1, 1}, {-1, 0}, {-1, -1}
	};

	for (auto &o : offsets) {
		int r = rank + o[0];
		int f = file + o[1];

		if (r >= 0 && r < size && f >= 0 && f < size) {
			attacks |= (1ULL << (r * size + f));
		}
	}
	return attacks;
}

void LookupTables::computeKingAttacks() {
	for (int i = 0; i < size*size; i++) {
		kingTable[i] = kingAttacksFrom(i);
	}
}

void LookupTables::computeAttackTables() {
	computeKnightAttacks();
	computeKingAttacks();
}
