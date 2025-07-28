#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include "move.hpp"

constexpr bool white_to_move = true;
constexpr bool black_to_move = false;
constexpr int bitboard_count = 12;
constexpr int occupancy_count = 3;
constexpr int a8 = 56;
constexpr int size = 8;
constexpr int board_size = 64;


enum Pieces {
	P, N, B, R, Q, K,
	p, n, b, r, q, k
};

enum Occupancies { white, black, all };

std::string squareNotation(int square);
int numberFromSquare(std::string);

class Board {
	private:
		void updateOccupancies();
		void setBit(uint64_t &bb, int square);
		
	public:
		uint64_t bitboards[bitboard_count] = {0};
		uint64_t occupancies[occupancy_count] = {0};
		bool side_to_move = white_to_move;
		
		void printBoard();
		void setBoard(const std::string& fen);
		void makeMove(const Move& move);
};

#endif
