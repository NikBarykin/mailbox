#include "test_letter_filter.h"
#include "server/source/letter_filter.h"

#include <iostream>
#include <string>
#include <cassert>
#include <vector>

#include "common/test/testing_utility.h"


namespace {
    void TestGeneral() {

        auto filter = LetterFilter::ParseFilter(R"((from == "A"          && body == "B")
                                                || (body == "X"          && date < 11.04.2013)
                                                || (date >= 12.12.2012   && from > "D")
                                                || "SUS" in body)");
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
    }

    void TestExtreme() {
        auto kind_filter = LetterFilter::ParseFilter("");

        for (int i = 0; i < 10; ++i) {
            Letter l = GenRandomLetter(i);
            assert(kind_filter(l));
        }
    }

    void TestExceptions() {
        std::string parse_error_filters[] = {"\'StringLiteral\"",
                                             "AMOGUS",
                                             "from body",
                                             "from < <",
                                             "body == \'Bad right parenthesis\')",
                                             "(body != \'Bad left parenthesis\'",
                                             "\'Literal can not be transformed to logical node\'"};

        for (const auto &bad_filter_str : parse_error_filters) {
            ASSERT_THROWS(LetterFilter::ParseFilter(bad_filter_str),
                          LetterFilter::ParseError);
        }

        std::string execution_error_filters[] = {"\'Different literal types\' == 11.09.2002",
                                                 "date in 21.03.2003"};

        for (const auto &bad_filter_str : execution_error_filters) {
            auto bad_filter = LetterFilter::ParseFilter(bad_filter_str);
            ASSERT_THROWS(bad_filter(GenRandomLetter(123)), LetterFilter::ExecutionError);
        }
    }
}


void TestLetterFilter() {
    TestGeneral();
    TestExtreme();
    TestExceptions();
    std::cerr << "TestLetterFilter: OK" << std::endl;
}
