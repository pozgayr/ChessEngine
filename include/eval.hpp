#ifndef EVAL_HPP
#define EVAL_HPP

#include "board.hpp"

constexpr int piece_values[] = {
    100, 320, 330, 500, 900, 20000,   
    -100, -320, -330, -500, -900, -20000  
};

int eval(Board &board);

#endif
