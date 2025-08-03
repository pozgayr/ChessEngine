#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "move.hpp"

constexpr int bitboard_count = 12;
constexpr int occupancy_count = 3;
constexpr int size = 8;
constexpr int board_size = 64;

enum Pieces {
	NONE = -1,
	P, N, B, R, Q, K,
	p, n, b, r, q, k
};

enum CastlingRights : unsigned int {
	WK = 1 << 0,
	WQ = 1 << 1,
	BK = 1 << 2,
	BQ = 1 << 3
};

enum Square {
    a1 = 0, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

struct Undo {
	int captured_piece;
	unsigned int prev_castling_rights;
	uint64_t prev_enpassant;
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

		std::vector<Move> move_stack;
		std::vector<Undo> undo_stack;
		
		void printBoard();
		void setBoard(const std::string& fen);
		void makeMove(const Move& move);
		void unmakeMove();
};

#endif
