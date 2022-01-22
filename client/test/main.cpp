#include "general/source/socket.h"
#include "test_build_query.h"
#include "test_cycle_outcome_processing.h"
#include "test_session.h"


int main() {
    WSALib wsa_lib;
    TestBuildQuery();
    TestCycleOutcomeProcessing();
    TestSession();
    return 0;
}
