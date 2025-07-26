#include "board.hpp"

void Board::setBit(uint64_t &bb, int square) {
	bb |= (1ULL << square);
}

void Board::printBoard() {
	char display[board_size];
	for (int i = 0; i < board_size; i++) display[i] = '.';

	const char piece_chars[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};

	for (int p = 0; p < bitboard_count; p++) {
		uint64_t bb = bitboards[p];

		while (bb) {
			int square = __builtin_ctzll(bb);

			display[square] = piece_chars[p];
			bb &= bb - 1;
		}
	}

	for (int rank = size - 1 ; rank >= 0; rank--) {
		for (int file = 0; file < size; file++) {
			std::cout << display[rank * size + file] << ' ';
		}
		std::cout << '\n';
	}

	std::cout << (side_to_move ? "White" : "Black") << " to move\n";
}

void Board::updateOccupancies() {
	occupancies[0] = bitboards[P] | bitboards[N] | bitboards[B] |
					 bitboards[R] | bitboards[Q] | bitboards[K];
					 
	occupancies[1] = bitboards[p] | bitboards[n] | bitboards[b] |
				     bitboards[r] | bitboards[q] | bitboards[k];

	occupancies[2] = occupancies[0] | occupancies[1];  
}

void Board::setBoard(const std::string& fen) {
	for (int i = 0; i < bitboard_count; i++) bitboards[i] = 0;
	for (int i = 0; i < occupancy_count; i++) occupancies[i] = 0;


	int square = a8;
	size_t i = 0;
	
	for (; i < fen.length(); i++) {
		char c = fen[i];

		if (c == ' ') {
			break;
		}
		else if (c == '/') {
			square -= size * 2;
			continue;
		}
		else if (isdigit(c)) {
			square += (c - '0');
		} else {
			switch(c) {
				case 'P': setBit(bitboards[P], square); break;
				case 'N': setBit(bitboards[N], square); break;
				case 'B': setBit(bitboards[B], square); break;
				case 'R': setBit(bitboards[R], square); break;
				case 'Q': setBit(bitboards[Q], square); break;
				case 'K': setBit(bitboards[K], square); break;
				case 'p': setBit(bitboards[p], square); break;
				case 'n': setBit(bitboards[n], square); break;
				case 'b': setBit(bitboards[b], square); break;
				case 'r': setBit(bitboards[r], square); break;
				case 'q': setBit(bitboards[q], square); break;
				case 'k': setBit(bitboards[k], square); break;
			}
			square++;
		}
	}
	i++;

	if (i < fen.length()) {
		side_to_move = (fen[i] == 'b') ? black_to_move : white_to_move;
	}

	updateOccupancies();
}
