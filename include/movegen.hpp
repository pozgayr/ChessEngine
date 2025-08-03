
#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>
#include "board.hpp"
#include "move.hpp"

using moveList = std::vector<Move>;

constexpr uint64_t rank1 = 0x00000000000000FFULL;
constexpr uint64_t rank2 = 0x000000000000FF00ULL;
constexpr uint64_t rank3 = 0x0000000000FF0000ULL;
constexpr uint64_t rank6 = 0x0000FF0000000000ULL;
constexpr uint64_t rank7 = 0x00FF000000000000ULL;
constexpr uint64_t rank8 = 0xFF00000000000000ULL;
constexpr uint64_t fileA = 0x0101010101010101ULL;
constexpr uint64_t fileH = 0x8080808080808080ULL;

const int queenDirections[][2] = {
		{0, 1}, {1, 0}, {0, -1}, {-1, 0},
		{1, 1}, {1, -1}, {-1, -1}, {-1, 1}
	};

const int rookDirections[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

const int bishopDirections[][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

struct MoveGenContext {
	const Board &board;
	moveList* out_moves;
	uint64_t &attack_mask;
	Color side;
};

class MoveGenerator {
	private:
		void pawnMoves(const Board& board, moveList &out_moves);
		void pawnAttackMoves(MoveGenContext &ctx);
		void knightMoves(MoveGenContext &ctx);
		void kingMoves(MoveGenContext &ctx);
		void rookMoves(MoveGenContext &ctx);
		void bishopMoves(MoveGenContext &ctx);
		void queenMoves(MoveGenContext &ctx);
		void castlingMoves(MoveGenContext &ctx);
		void traverseDirection(int from, const int directions[][2], 
							   int count, int piece, MoveGenContext &ctx);
						
	public:
		std::vector<Move> moves;
		uint64_t attack_mask;
		void genMoves(const Board &board);
		bool squareAttacked(const Board &board, Color side, const std::vector<int> &squares);
		bool kingInCheck(const Board &board, Color side);
		bool isMoveLegal(const Move &move, Board &board);
		
};

#endif
