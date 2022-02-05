#include "test_letter_filter.h"
#include "server/source/letter_filter.h"

#include <iostream>
#include <string>
#include <cassert>


namespace {
    void Test() {
        LetterFilter::LetterFilter filter("from == \"Nikita\" || from == \"Ivan\"");
        Letter valid1{"Nikita", "X", "Y"};
        Letter valid2{"Ivan", "1", "2"};
        Letter invalid{"Nik", "Z", "K"};
        assert(filter(valid1));
        assert(filter(valid2));
        assert(!filter(invalid));
    }
}

void TestLetterFilter() {
    Test();
    std::cerr << "TestLetterFilter: OK" << std::endl;
}
