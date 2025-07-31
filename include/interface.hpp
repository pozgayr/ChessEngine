#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "board.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include <string>
#include <vector>
#include <unordered_map>

const std::string quit_cmd = "quit";
const std::string quit_cmd_short = "q";
const std::string help_cmd = "help";
const std::string help_cmd_short = "h";
const std::string set_position_cmd = "position";
const std::string print_cmd = "print";
const std::string move_cmd = "move";
const std::string allmoves_cmd = "allmoves";
const std::string setside_cmd = "side";
const std::string start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string pawn_test = "rnbqkbnr/ppppp1pp/8/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 2";
const std::string knight_test = "n7/8/8/5N2/8/4N3/8/8 w - - 0 1";
const std::string king_test = "8/8/8/8/ppp5/pkp5/ppp5/7k w - - 0 1";
const std::string rook_test = "8/8/8/4R3/8/8/8/8 w - - 0 1";

extern std::unordered_map<std::string, std::string> positionPresets;

enum class CommandType { HELP, POSITION, PRINT, QUIT, MOVE, 
						ALLMOVES, SIDE, UNKNOWN };

CommandType getCommand(const std::string& cmd);

class Interface {
	public:
		void loop();
		static std::string squareToNotation(int square);
		static int notationToSquare(const std::string &square);
		static std::string moveToNotation(Move &move);
		static Move parseMoveInput(const std::string &input, Color side);

		void printMoves(std::vector<Move>& moves);

		
	private:
		Board board;
		MoveGenerator moveGen;

		void executeCommand(const std::vector<std::string>&, bool& quit);
		void cmdSetPosition(const std::vector<std::string>& args);
		void cmdPrint();
		void cmdHelp();
		void cmdMakeMove(const std::vector<std::string>& args);
		void cmdDisplayMoves();
		void cmdSwitchSide(const std::vector<std::string>& args);

		std::vector<std::string> split(const std::string& line, char delimiter = ' ');
};

#endif
