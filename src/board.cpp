#include "board.hpp"
#include "interface.hpp"
#include "movetables.hpp"

void Board::setBit(uint64_t &bb, int square) {
	bb |= (1ULL << square); //1ULL -  1 unsigned long long (64b)
}							//shifted right by the square positions

void Board::printBoard(int bitboard_index) {
	char display[board_size];
	for (int i = 0; i < board_size; i++) display[i] = ' ';

	const char piece_chars[12] = {'P','N','B','R','Q','K','p','n','b','r','q','k'};

	if (bitboard_index == -1) {
		for (int p = 0; p < bitboard_count; p++) {
			uint64_t bb = bitboards[p];
	
			while (bb) {
				int square = __builtin_ctzll(bb); //count trailing zeroes (hw instruct) 
				display[square] = piece_chars[p];
				bb &= bb - 1; //remove lsb
			}
		}	
	} else {
		if (bitboard_index < 0 || bitboard_index >= bitboard_count) {
			std::cout << "Invalid bitboard index\n";
			return;
		} 
		uint64_t bb = bitboards[bitboard_index];
		char piece = piece_chars[bitboard_index];
		while(bb) {
			int square = __builtin_ctzll(bb); 
			display[square] = piece;
			bb &= bb - 1;
		}
	}
	std::cout << " +---+---+---+---+---+---+---+---+\n";
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << " |";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            std::cout << " " << display[square] << " |";
        }
        std::cout << " " << (rank + 1) << "\n";
        std::cout << " +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << "   a   b   c   d   e   f   g   h\n";

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
	half_move_clock = 0;
	side_to_move = WHITE;
	undo_stack.clear();
	repetition_stack.clear();
	move_stack.clear();
	
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
	} else {
		idx++;
	}
	
	while (fen[idx] == ' ') {
		idx++;
	}
	
	std::string half_move_string;
	while (idx < fen.size() && fen[idx] != ' ') {
		half_move_string.push_back(fen[idx]);
		idx++;
	}
	std::cout << half_move_string << "\n";
	half_move_clock = std::stoi(half_move_string);
	
	updateOccupancies();
}

void Board::makeMove(const Move& move) {
	if (!(bitboards[move.piece] & (1ULL << move.from))) return;

	Undo u;
	u.captured_piece = NONE;
	u.prev_castling_rights = castling_rights;
	u.prev_enpassant = enpassant;
	u.prev_clock = half_move_clock;
	bitboards[move.piece] &= ~(1ULL << move.from); //remove from original position

	for (int i = 0; i < bitboard_count; i++) {
		if (bitboards[i] & (1ULL << move.to)) {
			u.captured_piece = i;
			bitboards[i] &= ~(1ULL << move.to); //remove piece if present at the target location
			break;
		}
	}
	
	bool capture = u.captured_piece != NONE;
	bool is_pawn = (move.piece == P) || (move.piece == p);

	if (capture || is_pawn) {
		half_move_clock = 0;
	}
	else {
		half_move_clock++;
	}
	

	if (!move.promotion) {
		bitboards[move.piece] |= (1ULL << move.to); //place piece
	} else {
		bitboards[move.promotion] |= (1ULL << move.to);
	}

	enpassant = 0ULL;
    if (move.piece == P && (move.to - move.from) == size * 2) { //hadle enpassant bitboard update
    	enpassant = (1ULL << (move.from + size)); 
    } else if (move.piece == p && (move.from - move.to) == size * 2) {
    	enpassant = (1ULL << (move.from - size));
    } 

    if (move.enpassant) {
    	if (side_to_move == WHITE) {
    		bitboards[p] &= ~((1ULL << move.to) >> size);
    		u.captured_piece = p;
    	} else {
    		bitboards[P] &= ~((1ULL << move.to) << size);
    		u.captured_piece = P;
    	}
    }

    if (move.castling) { //move rook
    	 switch (move.castling) {
	        case WK: 
	            bitboards[R] &= ~(1ULL << h1);
	            bitboards[R] |=  (1ULL << f1);
	            break;
	        case WQ: 
	            bitboards[R] &= ~(1ULL << a1);
	            bitboards[R] |=  (1ULL << d1);
	            break;
	        case BK: 
	            bitboards[r] &= ~(1ULL << h8);
	            bitboards[r] |=  (1ULL << f8);
	            break;
	        case BQ: 
	            bitboards[r] &= ~(1ULL << a8);
	            bitboards[r] |=  (1ULL << d8);
	            break;
   		}
    }
	undo_stack.push_back(u);
	move_stack.push_back(move);
	repetition_stack.push_back(posKey());
	
	castling_rights &= LookupTables::castlingRightsTable[move.from][move.to];
	side_to_move = static_cast<Color>(!side_to_move);
	updateOccupancies();	
}

void Board::unmakeMove() {
	Undo u = undo_stack.back();
	undo_stack.pop_back();
	Move move = move_stack.back();
	move_stack.pop_back();
	repetition_stack.pop_back();

	enpassant = u.prev_enpassant;
	castling_rights = u.prev_castling_rights;
	half_move_clock = u.prev_clock;

	int moved_piece = (move.promotion) ? move.promotion : move.piece;
	bitboards[moved_piece] &= ~(1ULL << move.to);

	bitboards[move.piece] |= (1ULL << move.from);

	 if (u.captured_piece != NONE) {
        if (move.enpassant) {
            int captured_square = (side_to_move == WHITE) ? move.to + size : move.to - size;
            bitboards[u.captured_piece] |= (1ULL << captured_square);
        } else {
            bitboards[u.captured_piece] |= (1ULL << move.to);
        }
    }
    if (move.castling) {
        if (move.to == g1) { 
            bitboards[R] &= ~(1ULL << f1);
            bitboards[R] |= (1ULL << h1);
        }
        else if (move.to == c1) {
            bitboards[R] &= ~(1ULL << d1);
            bitboards[R] |= (1ULL << a1);
        }
        else if (move.to == g8) {
            bitboards[r] &= ~(1ULL << f8);
            bitboards[r] |= (1ULL << h8);
        }
        else if (move.to == c8) { 
            bitboards[r] &= ~(1ULL << d8);
            bitboards[r] |= (1ULL << a8);
        }
    }
    side_to_move = static_cast<Color>(!side_to_move);
    updateOccupancies();
}

std::string Board::signature() const {
    std::string sig;
    for (int i = 0; i < bitboard_count; i++) {
        sig += std::to_string(bitboards[i]) + "|";
    }
    sig += std::to_string(enpassant) + "|";
    sig += std::to_string(castling_rights) + "|";
    sig += std::to_string(side_to_move);
    return sig;
}

uint64_t Board::posKey() {
	uint64_t h = 14695981039346656037ULL;
	constexpr uint64_t FNV_PRIME = 1099511628211ULL; 

	auto mix = [&](uint8_t x) {
		h ^= x;
		h *= FNV_PRIME;
	};

	for (int piece = 0; piece < bitboard_count; piece++) {
		uint64_t bb = bitboards[piece];
		while (bb) {
			int square = __builtin_ctzll(bb);
			bb &= bb - 1;
			mix(piece);
			mix(square);
		}
	}
	mix(side_to_move);
	mix(castling_rights & 0x0F);

	return h;
}
