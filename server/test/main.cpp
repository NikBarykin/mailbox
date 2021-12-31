#include "test_server.h"
#include "test_database.h"
#include "test_query_processor.h"


int main() {
    TestDatabase();
    TestQueryProcessor();
    return 0;
}
