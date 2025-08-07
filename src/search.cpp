#include "search.hpp"

Move Search::search(Board &board, int depth) {
	BestMove best_move;
	
	best_move.score = (board.side_to_move) ? min_val : max_val;
	
	MoveGenerator movegen;
	movegen.genMoves(board);
	int alpha = min_val;
	int beta = max_val;

	for (Move &m : movegen.moves) {
		board.makeMove(m);
		int score = (board.side_to_move == WHITE) ? maxi(board, depth - 1, alpha, beta) 
												  : mini(board, depth - 1, alpha, beta);
		board.unmakeMove();

		if (board.side_to_move == WHITE) {
			if (score > best_move.score) {
				best_move.move = m;
				best_move.score = score;
			}
		} else {
			if (score < best_move.score) {
				best_move.move = m;
				best_move.score = score;
			}
		}
	}
	std::cout << best_move.score << "\n";
	std::cout << best_move.move.from << best_move.move.to << "\n";
	return best_move.move;
}

int Search::maxi(Board &board, int depth, int alpha, int beta) {
	int best_score = min_val;

	if (depth < 1) {
		return eval(board);
	}

	MoveGenerator movegen;
	movegen.genMoves(board);
	
	for (Move &m : movegen.moves) {
		board.makeMove(m);
		int move_score = mini(board, depth - 1, alpha, beta);
		board.unmakeMove();
		best_score = std::max(best_score, move_score);
		alpha = std::max(best_score, alpha);
		if (alpha >= beta) break;
	}
	return best_score;
}

int Search::mini(Board &board, int depth, int alpha, int beta) {
	int best_score = max_val;

	if (depth < 1) {
		return eval(board);
	}

	MoveGenerator movegen;
	movegen.genMoves(board);

	for (Move &m : movegen.moves) {
		board.makeMove(m);
		int move_score = maxi(board, depth - 1, alpha, beta);
		board.unmakeMove();
		best_score = std::min(best_score, move_score);
		beta = std::min(best_score, beta);
		if (beta <= alpha) break;
	}
	return best_score;
}
