#include "interface.hpp"
#include "board.hpp"

CommandType getCommand(const std::string& cmd) {
    if (cmd == help_cmd || cmd == help_cmd_short) return CommandType::HELP;
    if (cmd == set_position_cmd) return CommandType::POSITION;
    if (cmd == print_cmd) return CommandType::PRINT;
    if (cmd == quit_cmd || cmd == quit_cmd_short) return CommandType::QUIT;
    return CommandType::UNKNOWN;
}

void Interface::loop() {
	std::string current_command;
	bool quit = false;

	while (!quit) {
		std::cout << "> ";
		if (!std::getline(std::cin, current_command)) break;

		if (current_command.empty()) continue;
		
		std::vector<std::string> parsed_command = split(current_command, ' ');
	
		executeCommand(parsed_command, quit);
	}
}

void Interface::executeCommand(const std::vector<std::string>& args, bool& quit) {
	if (args.empty()) return;
	
	const std::string& cmd = args.at(0);
	CommandType cmd_type = getCommand(cmd);

	switch (cmd_type) {
		case CommandType::HELP:
			cmdHelp();
			break;
		case CommandType::POSITION:
			cmdSetPosition(args);
			break;
		case CommandType::PRINT:
			cmdPrint();
			break;
		case CommandType::QUIT:
			std::cout << "Exiting...\n";
		    quit = true;
		    break;
		case CommandType::UNKNOWN:
		default:
			std::cout << "Unknown command: " << args.at(0) << "\n";
			break;
	}
}

std::vector<std::string> Interface::split(const std::string& line, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	bool in_quotes = false;

	for (size_t i = 0; i < line.length(); i++) {
		char c = line[i];

		if (c == '"') {
			in_quotes = !in_quotes;
			continue;
		} else if (c == delimiter && !in_quotes) {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else {
			token.push_back(c);
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	return tokens;
}

void Interface::cmdSetPosition(const std::vector<std::string>& args) {
	if (args.size() < 2) {
		std::cout << "Error: position requires at least 2 arguments\n";
		return;
	}
	if (args.at(1) == "startpos") {
		board.setBoard(start_pos);
	} else if (args.at(1) == "fen") {
		if (args.size() < 3) {
			std::cout << "Error: position fen requires at least 3 arguments\n";
			return;
		}
		board.setBoard(args.at(2)); // TODO: add fen validity checker
	} else {
		std::cout << "Error: unknown position argument" << args.at(1) << "\n";	
	}
}

void Interface::cmdHelp() {
    std::cout << "Commands:\n"
              << "  help / h               - Show help\n"
              << "  position startpos      - Load starting position\n"
              << "  position fen \"<FEN>\" - Load position from FEN\n"
              << "  print                  - Print board\n"
              << "  quit / q               - Exit\n";
}

void Interface::cmdPrint() {
	board.printBoard();
}
