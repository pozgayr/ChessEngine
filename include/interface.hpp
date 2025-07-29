#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "board.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include <string>
#include <vector>

const std::string quit_cmd = "quit";
const std::string quit_cmd_short = "q";
const std::string help_cmd = "help";
const std::string help_cmd_short = "h";
const std::string set_position_cmd = "position";
const std::string print_cmd = "print";
const std::string move_cmd = "move";
const std::string allmoves_cmd = "allmoves";
const std::string start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum class CommandType { HELP, POSITION, PRINT, QUIT, MOVE, ALLMOVES, UNKNOWN };

CommandType getCommand(const std::string& cmd);

class Interface {
	public:
		void loop();
		static std::string squareToNotation(int square);
		static int notationToSquare(std::string &square);

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

		std::vector<std::string> split(const std::string& line, char delimiter = ' ');
};

#endif
