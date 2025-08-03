#ifndef PERFT_HPP
#define PERFT_HPP

#include "board.hpp"
#include "movegen.hpp"
#include <iostream>

uint64_t perft(Board &board, int depth);
void perftDivide(Board &board, int depth);
void perftDebug(Board &board, int depth, std::string debug);

#endif 
