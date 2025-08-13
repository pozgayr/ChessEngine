#include "search.hpp"

Move Search::search(Board &board, int depth) {
	BestMove best_move;
	
	best_move.score = (board.side_to_move == WHITE) ? min_val : max_val;
	
	MoveGenerator movegen;
	movegen.genMoves(board);
	if (!movegen.moves.empty()) best_move.move = movegen.moves[0];
	
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
		if (board.side_to_move == WHITE) alpha = std::max(alpha, score);
        else beta = std::min(beta, score);
		if (alpha >= beta) break;
	}
	return best_move.move;
}

int Search::maxi(Board &board, int depth, int alpha, int beta) {
	assert(board.side_to_move == WHITE);
	int best_score = min_val;

	if (depth < 1) {
		return eval(board);
	}

	MoveGenerator movegen;
	movegen.genMoves(board);

	if (movegen.moves.empty()) {
		if (movegen.kingInCheck(board, WHITE)) {
			return -mate_score - depth; 
		}
		return 0;
	}
	sortMoves(movegen.moves);
	
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
	assert(board.side_to_move == BLACK);
	int best_score = max_val;

	if (depth < 1) {
		return eval(board);
	}

	MoveGenerator movegen;
	movegen.genMoves(board);

	if (movegen.moves.empty()) {
		if (movegen.kingInCheck(board, BLACK)) {
			return mate_score + depth; 
		}
		return 0;
	}
	sortMoves(movegen.moves);

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

void Search::sortMoves(moveList &moves) {
	std::sort(moves.begin(), moves.end(), [this](const Move &a, const Move &b) {
		return scoreMove(a) > scoreMove(b);
	});
}
