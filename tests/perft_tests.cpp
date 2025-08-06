#include <catch2/catch_test_macros.hpp>
#include "board.hpp"
#include "perft.hpp"
#include "interface.hpp"

struct PerftFixture {
    PerftFixture() {
        board.setBoard(start_pos);
    }
    Board board;
};

TEST_CASE_METHOD(PerftFixture, "Perft starting position", "[perft]") {
    REQUIRE(perft(board, 1) == 20);
    REQUIRE(perft(board, 2) == 400);
    REQUIRE(perft(board, 3) == 8902);
    REQUIRE(perft(board, 4) == 197281);
}

TEST_CASE_METHOD(PerftFixture, "Kiwipete position", "[perft]") {
    board.setBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    REQUIRE(perft(board, 1) == 48);
    REQUIRE(perft(board, 2) == 2039);
    REQUIRE(perft(board, 3) == 97862);
}

TEST_CASE_METHOD(PerftFixture, "Position 3 from chesswiki", "[perft]") {
	board.setBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");
	REQUIRE(perft(board, 1) == 14);
	REQUIRE(perft(board, 2) == 191);
	REQUIRE(perft(board, 3) == 2812);
}

TEST_CASE_METHOD(PerftFixture, "Mirror position", "[perft]") {
	board.setBoard("r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ");
	REQUIRE(perft(board, 1) == 6);
	REQUIRE(perft(board, 2) == 264);
	REQUIRE(perft(board, 3) == 9467);
}
