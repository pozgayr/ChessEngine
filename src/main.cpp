#include "interface.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "movetables.hpp"
#include "perft.hpp"


int main() {

	LookupTables::computeAttackTables();
	Board board;
	board.setBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	perftDebug(board, 3, "c2c4");
	
	return 0;
}
