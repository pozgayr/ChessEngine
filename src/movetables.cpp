#include "movetables.hpp"

uint64_t LookupTables::knightTable[size*size];
uint64_t LookupTables::kingTable[size*size];
unsigned int LookupTables::castlingRightsTable[size*size][size*size];

void LookupTables::computeCastlingRights() {
	int white_back_rank = 0;
	int black_back_rank = size - 1;
	int white_king = white_back_rank * size + size / 2;
	int black_king = black_back_rank * size + size / 2;

	int white_rook_ks = white_back_rank * size + (size - 1);
	int white_rook_qs = white_back_rank * size + 0;
	int black_rook_ks = black_back_rank * size + (size - 1);
	int black_rook_qs = black_back_rank * size + 0;

	for (int i = 0; i < size*size; i++) {
		for (int j = 0; j < size*size; j++) {
			castlingRightsTable[i][j] = WK | WQ | BK | BQ;
		}
	}

	for (int to = 0; to < size*size; to++) {
		castlingRightsTable[white_king][to] &= ~(WK | WQ);
		castlingRightsTable[black_king][to] &= ~(BK | BQ);
	}

	for (int to = 0; to < size*size; to++) {
		castlingRightsTable[white_rook_ks][to] &= ~WK;
		castlingRightsTable[white_rook_qs][to] &= ~WQ;
		castlingRightsTable[black_rook_ks][to] &= ~BK;
		castlingRightsTable[black_rook_qs][to] &= ~BQ;
	}

	for (int from = 0; from < size*size; from++) {
		castlingRightsTable[from][white_rook_ks] &= ~WK;
		castlingRightsTable[from][white_rook_qs] &= ~WQ;
		castlingRightsTable[from][black_rook_ks] &= ~BK;
		castlingRightsTable[from][black_rook_qs] &= ~BQ;
	}
}

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
	computeCastlingRights();
}
