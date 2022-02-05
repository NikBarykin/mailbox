#include "test_letter_filter.h"
#include "server/source/letter_filter.h"

#include <iostream>
#include <string>
#include <cassert>
#include <vector>


namespace {
    void Test() {
        auto filter = LetterFilter::MakeFilter(R"((from == "Nikita" && body == "AMOGUS")
                                                  || (from == "Akim" && body == "SUS"))");
        assert(filter({"Nikita", "1", "AMOGUS"}));
        assert(!filter({"Akim", "2", "AMOGUS"}));
        assert(filter({"Akim", "3", "SUS"}));
        assert(!filter({"Nikita", "4", "SUS"}));
    }
}

void TestLetterFilter() {
    Test();
    std::cerr << "TestLetterFilter: OK" << std::endl;
}
