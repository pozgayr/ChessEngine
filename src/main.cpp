#include "interface.hpp"
#include "board.hpp"
#include "movegen.hpp"


int main() {

	Board board;
	board.setBoard("8/8/8/8/8/8/1P6/8 b KQkq - 0 1");
	board.printBoard();
	MoveGenerator moveGen;
	moveGen.pawnMoves(board);

	for (auto &m : moveGen.moves) {
	    board.makeMove(m);
	    board.printBoard();
	}

	std::cout << squareNotation(63) << '\n';
	return 0;
}
