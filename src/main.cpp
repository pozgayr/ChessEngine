#include "board.hpp"

int main() {
	Board board;
	
	board.setBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.printBoard();
	
	return 0;
}
