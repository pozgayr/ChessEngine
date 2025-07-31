#include "interface.hpp"

std::unordered_map<std::string, std::string> positionPresets = {
    {"startpos", start_pos},
    {"pawn", pawn_test},
    {"knight", knight_test},
    {"king", king_test}
};

CommandType getCommand(const std::string& cmd) {
    if (cmd == help_cmd || cmd == help_cmd_short) return CommandType::HELP;
    if (cmd == set_position_cmd) return CommandType::POSITION;
    if (cmd == print_cmd) return CommandType::PRINT;
    if (cmd == quit_cmd || cmd == quit_cmd_short) return CommandType::QUIT;
    if (cmd == move_cmd) return CommandType::MOVE;
    if (cmd == allmoves_cmd) return CommandType::ALLMOVES;
    if (cmd == setside_cmd) return CommandType::SIDE;
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
		case CommandType::SIDE:
			cmdSwitchSide(args);
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

void Interface::cmdSwitchSide(const std::vector<std::string>& args) {
	if (args.size() < 2) {
		std::cout << "Error: side requires exactly 2 arguments\n";
		return;
	}

	if (args.at(1) == "white" || args.at(1) == "w") {
		board.side_to_move = WHITE;
		return;
	} else if (args.at(1) == "black" || args.at(1) == "b") {
		board.side_to_move = BLACK;
		return;
	}
	std::cout << "Error unknown side argument\n";
} 

void Interface::cmdSetPosition(const std::vector<std::string>& args) {
	if (args.size() < 2) {
		std::cout << "Error: position requires at least 2 arguments\n";
		return;
	}

	auto it = positionPresets.find(args.at(1));
	if (it != positionPresets.end()) {
		board.setBoard(it->second);
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
              << "  move fromtop           - Make a move proting to piece p\n"
              << "  eg.e7e8b promoting to bishop\n"
              //TODO: if no argument is last promote to queen
              << "  allmoves               - Display all moves\n"
              << "  side w/b               - Set side to move\n"
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
	
	Move move = parseMoveInput(args.at(1), board.side_to_move);

	 if (move.from == -1 || move.to == -1) {
   		std::cout << "Invalid squares entered\n";
        return;
   	}

	moveGen.genMoves(board);
	
	for (const auto& m : moveGen.moves) {
	    if (m == move) {
	        board.makeMove(m);
	        board.printBoard();
	        return;
	    }
	}
	std::cout << "Invalid move\n";
}

void Interface::cmdDisplayMoves() {
	moveGen.genMoves(board);
	printMoves(moveGen.moves);
}

Move Interface::parseMoveInput(const std::string &input, Color side) {
	Move m;

	m.from = notationToSquare(input.substr(0, 2));
    m.to = notationToSquare(input.substr(2, 2));
	m.promotion = 0;
	m.enpassant = 0;
   	
    if (input.size() == 5) {
       switch (input[4]) {
           case 'q': m.promotion = (side == WHITE) ? Q : q; break;
           case 'r': m.promotion = (side == WHITE) ? R : r; break;
           case 'b': m.promotion = (side == WHITE) ? B : b; break;
           case 'n': m.promotion = (side == WHITE) ? N : n; break;
       }
   }

   return m;
}

std::string Interface::squareToNotation(int square) {
	int file = square % 8;
	int rank = square / 8;
	
	char file_char = 'a' + file;
	char rank_char = '1' + rank;

	return std::string{file_char, rank_char}; 
}

std::string Interface::moveToNotation(Move &move) {
	std::string moveNotation = squareToNotation(move.from) + squareToNotation(move.to);

	if (move.promotion) {
		char promoChar;
		switch (move.promotion) {
	        case Q: case q: promoChar = 'q'; break;
	        case R: case r: promoChar = 'r'; break;
	        case B: case b: promoChar = 'b'; break;
	        case N: case n: promoChar = 'n'; break;
	        default: promoChar = '?'; break;
		}
		moveNotation += promoChar;
	}
	return moveNotation;
}

int Interface::notationToSquare(const std::string &square) { 
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
		std::cout << moveToNotation(moves[i]) << "\n";	
	}
}
