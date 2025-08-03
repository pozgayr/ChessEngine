#ifndef MOVE_HPP
#define MOVE_HPP

#include <string>

struct Move {
	int from;
	int to;
	int piece;
	int promotion = 0;
	bool enpassant = false;
	unsigned int castling = 0;

 	bool operator==(const Move &other) const {
	    return from == other.from &&
	        to == other.to &&
	        promotion == other.promotion;
    }
    
	std::string toString() const {
		static const char files[] = "abcdefgh";
		static const char ranks[] = "12345678";
		return std::string() +
		    files[from % 8] + ranks[from / 8] +
		    files[to % 8] + ranks[to / 8];
	}
};

#endif
