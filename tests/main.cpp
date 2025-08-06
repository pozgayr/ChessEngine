#define CATCH_CONFIG_RUNNER
#include <catch2/catch_session.hpp> 
#include "movetables.hpp"

int main(int argc, char* argv[]) {
    LookupTables::computeAttackTables();

    Catch::Session session;
    return session.run(argc, argv);
}
