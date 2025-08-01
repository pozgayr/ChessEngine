#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include "move.hpp"

constexpr int bitboard_count = 12;
constexpr int occupancy_count = 3;
constexpr int a8 = 56;
constexpr int size = 8;
constexpr int board_size = 64;


enum Pieces {
	P, N, B, R, Q, K,
	p, n, b, r, q, k
};

enum CastlingRights {
	WHITE_KING_SIDE = 1,
	WHITE_QUEEN_SIDE = 2,
	BLACK_KING_SIDE = 4,
	BLACK_QUEEN_SIDE = 8
};

enum Occupancies { black, white, all };
enum Color { WHITE = 1, BLACK = 0 };

std::string squareNotation(int square);
int numberFromSquare(std::string);

class Board {
	private:
		void updateOccupancies();
		void setBit(uint64_t &bb, int square);
		
	public:
		uint64_t bitboards[bitboard_count] = {0};
		uint64_t occupancies[occupancy_count] = {0};
		uint64_t enpassant = 0;
		unsigned int castling_rights = 0;
		Color side_to_move = WHITE;
		
		void printBoard();
		void setBoard(const std::string& fen);
		void makeMove(const Move& move);
};

#endif
