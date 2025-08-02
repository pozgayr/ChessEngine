#include "board.hpp"
#include "interface.hpp"
#include "movetables.hpp"

void Board::setBit(uint64_t &bb, int square) {
	bb |= (1ULL << square); //1ULL -  1 unsigned long long (64b)
}							//shifted right by the square positions

void Board::printBoard() {
	char display[board_size];
	for (int i = 0; i < board_size; i++) display[i] = '.';

	const char piece_chars[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};

	for (int p = 0; p < bitboard_count; p++) {
		uint64_t bb = bitboards[p];

		while (bb) {
			int square = __builtin_ctzll(bb); //count trailing zeroes (hw instruct) 
			display[square] = piece_chars[p];
			bb &= bb - 1; //remove lsb
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
	occupancies[white] = bitboards[P] | bitboards[N] | bitboards[B] |
					 bitboards[R] | bitboards[Q] | bitboards[K];
					 
	occupancies[black] = bitboards[p] | bitboards[n] | bitboards[b] |
				     bitboards[r] | bitboards[q] | bitboards[k];

	occupancies[all] = occupancies[white] | occupancies[black];  
}

void Board::setBoard(const std::string& fen) {
	for (int i = 0; i < bitboard_count; i++) bitboards[i] = 0;
	for (int i = 0; i < occupancy_count; i++) occupancies[i] = 0;
	castling_rights = 0U;
	enpassant = 0ULL;
	side_to_move = WHITE;
	
	int square = a8;
	size_t idx = 0;

	while (idx < fen.length() && fen[idx] != ' ') {
		char c = fen[idx];

		if (c == '/') {
			square -= size * 2;
		} else if (isdigit(c)) {
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
		idx++;
	}
	
	while (fen[idx] == ' ') {
		idx++;
	}

	if (idx < fen.length()) {
		side_to_move = (fen[idx] == 'b') ? BLACK : WHITE;
		idx++;
	}

	while (fen[idx] == ' ') {
		idx++;
	}
	
	while (idx < fen.size() && fen[idx] != ' ') {
	    switch (fen[idx]) {
	    	case 'K' : castling_rights |= WK; break;
	    	case 'Q' : castling_rights |= WQ; break;
	    	case 'k' : castling_rights |= BK; break;
	    	case 'q' : castling_rights |= BQ; break;
	    	default: break;
	    }
	    idx++;
	}
	
	while (fen[idx] == ' ') {
		idx++;
	}

	if (fen[idx] != '-') {
		std::string enpassant_square_str = fen.substr(idx, 2);
		int enpassant_square = Interface::notationToSquare(enpassant_square_str);
		enpassant = 1ULL << enpassant_square;
		idx += 2;
	}

	updateOccupancies();
}

void Board::makeMove(const Move& move) {

	if (bitboards[move.piece] & (1ULL << move.from)) {
		bitboards[move.piece] &= ~(1ULL << move.from); //remove from original position

		for (int i = 0; i < bitboard_count; i++) {
			if (bitboards[i] & (1ULL << move.to)) {
				bitboards[i] &= ~(1ULL << move.to); //remove piece if present at the target location
				break;
			}
		}

		if (!move.promotion) {
			bitboards[move.piece] |= (1ULL << move.to); //place piece
		} else {
			bitboards[move.promotion] |= (1ULL << move.to);
		}
		

	    if (move.piece == P && (move.to - move.from) == size * 2) { //hadle enpassant bitboard update
	    	enpassant = (1ULL << (move.from + size)); 
	    } else if (move.piece == p && (move.from - move.to) == size * 2) {
	    	enpassant = (1ULL << (move.from - size));
	    }

	    if (move.enpassant) {
	    	if (side_to_move == WHITE) {
	    		bitboards[p] &= ~((1ULL << move.to) >> size);
	    	} else {
	    		bitboards[P] &= ~((1ULL << move.to) << size);
	    	}
	    }

		castling_rights &= LookupTables::castlingRightsTable[move.from][move.to];
		std::cout << castling_rights << "\n";
		side_to_move = (side_to_move == WHITE) ? BLACK : WHITE;
		updateOccupancies();
	}
	
}
