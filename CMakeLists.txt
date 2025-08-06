cmake_minimum_required(VERSION 3.10)

project(ChessEngine VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Engine library (excluding main.cpp)
add_library(chess_engine_lib
    src/board.cpp
    src/interface.cpp
    src/movegen.cpp
    src/movetables.cpp
    src/perft.cpp
)
target_include_directories(chess_engine_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Main executable
add_executable(chess_engine src/main.cpp)
target_link_libraries(chess_engine PRIVATE chess_engine_lib)

# --- Testing setup ---
include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# Tests executable
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE chess_engine_lib Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)
