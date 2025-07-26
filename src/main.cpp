
#include "board.hpp"

int main() {
	Board board;
	
	board.setBoard("6k1/1pb3p1/2p4p/2Pp4/Pp1B2nq/3B3P/2Q1N1P1/5K2 b - - 0 1");
	board.printBoard();
	
	return 0;
}
