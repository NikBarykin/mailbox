#include "test_letter_filter.h"
#include "server/source/letter_filter.h"

#include <iostream>
#include <string>
#include <cassert>
#include <vector>

#include "common/test/testing_utility.h"


namespace {
    // TODO: test exceptions
    void TestGeneral() {

        auto filter = LetterFilter::ParseFilter(R"((from == "A"          && body == "B")
                                                || (body == "X"          && date < 11.04.2013)
                                                || (date >= 12.12.2012   && from > "D")
                                                || "SUS" IN body)");
        Letter valid_letters[] = {{"A", "_", "B", {10, 03, 2100}},
                                  {"C", "_", "X", {10, 04, 2013}},
                                  {"E", "_", "Z", {12, 12, 2012}},
                                  {"D", "_", "AMOGSUS", {11, 11, 2011}}};

        for (Letter valid : valid_letters) {
            assert(filter(valid));
        }

        Letter invalid_letters[] = {{"B", "_", "C", {9, 9, 2003}},
                                    {"A", "_", "X", {11, 04, 2013}},
                                    {"X", "_", "Z", {11, 12, 2012}},
                                    {"D", "_", "S U S", {05, 05, 2005}}};

        for (Letter invalid : invalid_letters) {
            assert(!filter(invalid));
        }
//
//        auto filter = LetterFilter::ParseFilter(R"((from == "Nikita" && body == "AMOGUS")
//                                                  || (from == "Akim" && body == "SUS"))");
//        assert(filter("Nikita 1 AMOGUS"_l));
//        assert(!filter("Akim 2 AMOGUS"_l));
//        assert(filter("Akim 3 SUS"_l));
//        assert(!filter("Nikita 4 SUS"_l));
    }

    void TestExtreme() {
        auto kind_filter = LetterFilter::ParseFilter("");

        for (int i = 0; i < 10; ++i) {
            Letter l = GenRandomLetter(i);
            assert(kind_filter(l));
        }
    }
}

void TestLetterFilter() {
    TestGeneral();
    TestExtreme();
    std::cerr << "TestLetterFilter: OK" << std::endl;
}
