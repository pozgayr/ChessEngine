#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <iostream>
#include <string>

const bool white_to_move = true;
const bool black_to_move = false;
const int bitboard_count = 12;
const int occupancy_count = 3;
const int a8 = 56;
const int size = 8;
const int board_size = 64;


enum Pieces {
	P, N, B, R, Q, K,
	p, n, b, r, q, k
};

class Board {
	private:
		uint64_t bitboards[bitboard_count] = {0};
		uint64_t occupancies[occupancy_count] = {0};
		bool side_to_move = white_to_move;

		void updateOccupancies();
		void setBit(uint64_t &bb, int square);
		
	public:
		void printBoard();
		void setBoard(const std::string& fen);
};

#endif
