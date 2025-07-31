#include "interface.hpp"
#include "board.hpp"
#include "movegen.hpp"
#include "movetables.hpp"


int main() {

	LookupTables::computeAttackTables();
	Interface interface;
	interface.loop();

	return 0;
}
