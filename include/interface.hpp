#include "board.hpp"
#include <string>
#include <vector>

const std::string quit_cmd = "quit";
const std::string quit_cmd_short = "q";
const std::string help_cmd = "help";
const std::string help_cmd_short = "h";
const std::string set_position_cmd = "position";
const std::string print_cmd = "print";
const std::string start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum class CommandType { HELP, POSITION, PRINT, QUIT, UNKNOWN };

CommandType getCommand(const std::string& cmd);

class Interface {
	public:
		void loop();

	private:
		Board board;

		void executeCommand(const std::vector<std::string>&, bool& quit);
		void cmdSetPosition(const std::vector<std::string>& args);
		void cmdPrint();
		void cmdHelp();

		std::vector<std::string> split(const std::string& line, char delimiter = ' ');
};
