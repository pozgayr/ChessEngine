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
const std::string pos_cmd = "position";
const std::string pos_cmd_short = "pos";
const std::string print_cmd = "print";
const std::string move_cmd = "move";
const std::string allmoves_cmd = "allmoves";
const std::string setside_cmd = "side";
const std::string start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string pawn_test = "8/3P4/8/8/8/8/8/8 w - - 0 1";
const std::string knight_test = "n7/8/8/5N2/8/4N3/8/8 w q - 0 1";
const std::string king_test = "8/8/8/8/ppp5/pkp5/ppp5/7k w Kk - 0 1";
const std::string rook_test = "8/8/8/2rR2r1/3B4/8/8/8 w Qq - 0 1";
const std::string bishop_test = "1r6/8/5r2/4B3/3Q1Q2/8/8/8 w Kkq - 0 1";
const std::string queen_test = "4p2Q/6Q1/8/8/7p/8/8/8 b KQq - 0 1";
const std::string castling_test = "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1";

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
