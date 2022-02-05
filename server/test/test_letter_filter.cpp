#include "test_letter_filter.h"
#include "server/source/letter_filter.h"

#include <iostream>
#include <string>
#include <cassert>
#include <vector>


namespace {
    void TestGeneral() {
        auto filter = LetterFilter::ParseFilter(R"((from == "Nikita" && body == "AMOGUS")
                                                  || (from == "Akim" && body == "SUS"))");
        assert(filter({"Nikita", "1", "AMOGUS"}));
        assert(!filter({"Akim", "2", "AMOGUS"}));
        assert(filter({"Akim", "3", "SUS"}));
        assert(!filter({"Nikita", "4", "SUS"}));
    }

    void TestExtreme() {
//        auto kind_filter = LetterFilter::
    }
}

void TestLetterFilter() {
    TestGeneral();
    TestExtreme();
    std::cerr << "TestLetterFilter: OK" << std::endl;
}
