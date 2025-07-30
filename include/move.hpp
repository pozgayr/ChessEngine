#ifndef MOVE_HPP
#define MOVE_HPP

struct Move {
	int from;
	int to;
	int piece;
	int promotion;
	bool enpassant;

 	bool operator==(const Move &other) const {
	    return from == other.from &&
	        to == other.to &&
	        promotion == other.promotion;
    }
};

#endif
