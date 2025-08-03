cmake_minimum_required(VERSION 3.10)

project(ChessEngine VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
	src/main.cpp
	src/board.cpp
	src/interface.cpp
	src/movegen.cpp
	src/movetables.cpp
	src/perft.cpp
)

add_executable(chess_engine ${SOURCES})
