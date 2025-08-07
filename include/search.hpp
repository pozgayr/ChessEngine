#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "board.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "eval.hpp"
#include <limits>

struct BestMove {
	Move move;
	int score;	
};

class Search {
	public:
		Move search(Board &board, int depth);

	private: 
		int max_val = std::numeric_limits<int>::max();
		int min_val = std::numeric_limits<int>::min();
		int mate_score = max_val;
		int maxi(Board &board, int depth, int alpha, int beta);
		int mini(Board &board, int depth, int alpha, int beta);
};

#endif
