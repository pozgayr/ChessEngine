#include "interface.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "movetables.hpp"


int main() {


	
	LookupTables::computeAttackTables();

	Board board;
	MoveGenerator movegen;
	
	board.setBoard("8/8/8/8/8/8/r7/8 w - - 0 1");
	std::cout << movegen.squareAttacked(board, BLACK, 9) << "\n";
	
	return 0;
}
