#include "interface.hpp"

CommandType getCommand(const std::string& cmd) {
    if (cmd == help_cmd || cmd == help_cmd_short) return CommandType::HELP;
    if (cmd == set_position_cmd) return CommandType::POSITION;
    if (cmd == print_cmd) return CommandType::PRINT;
    if (cmd == quit_cmd || cmd == quit_cmd_short) return CommandType::QUIT;
    if (cmd == move_cmd) return CommandType::MOVE;
    if (cmd == allmoves_cmd) return CommandType::ALLMOVES;
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
		case CommandType::MOVE:
			cmdMakeMove(args);
			break;
		case CommandType::ALLMOVES:
			cmdDisplayMoves();
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
              << "  move fromto            - Make a move eg.e4e5\n"
              << "  allmoves               - display all moves\n"
              << "  quit / q               - Exit\n";
}

void Interface::cmdPrint() {
	board.printBoard();
}

void Interface::cmdMakeMove(const std::vector<std::string>& args) {
	if (args.size() != 2) {
		std::cout << "Error: move requires 2 arguments\n";
		return;
	}
	
	std::string move = args.at(1);
	std::string from_str = move.substr(0,2);
	std::string to_str = move.substr(2,2);

	int from = notationToSquare(from_str);
	int to = notationToSquare(to_str);

	if (from == -1 || to == -1) {
		std::cout << "Invalid squares: " << from_str << " " << to_str << "\n";
        return;
	}

	moveGen.genMoves(board);
	
	for (const auto& m : moveGen.moves) {
	    if (m.from == from && m.to == to) {
	        board.makeMove(m);
	        std::cout << "Move applied: " << from_str << to_str << "\n";
	        return;
	    }
	}
	std::cout << "Invalid move\n";
}

void Interface::cmdDisplayMoves() {
	moveGen.genMoves(board);
	printMoves(moveGen.moves);
}

std::string Interface::squareToNotation(int square) {
	int file = square % 8;
	int rank = square / 8;
	
	char file_char = 'a' + file;
	char rank_char = '1' + rank;

	return std::string{file_char, rank_char}; 
}

int Interface::notationToSquare(std::string &square) { 
	if (square.size() != 2) return -1;
	
	char file_char = square.at(0);
	char rank_char = square.at(1);

	if (file_char < 'a' || file_char > 'h') return -1;
	if (rank_char < '1' || rank_char > '8') return -1;

	int file = file_char - 'a';
	int rank = rank_char - '1';

	return rank * 8 + file;
}

void Interface::printMoves(std::vector<Move>& moves) {
	for (size_t i = 0; i < moves.size(); i++) {
		std::cout << squareToNotation(moves[i].from) << squareToNotation(moves[i].to) << "\n";
	}
}
