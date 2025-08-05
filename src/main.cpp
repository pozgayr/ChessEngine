#include "interface.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "movetables.hpp"
#include "perft.hpp"


int main() {

	LookupTables::computeAttackTables();

	Interface interface;
	interface.loop();
	/*Board board;
	board.setBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

	Board copy = board;
	perft(board, 3);

	if (!(board == copy)) {
		std::cout << "error\n";
	}*/

	return 0;
}
