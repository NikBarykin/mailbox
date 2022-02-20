#include "test_letter.h"

#include <iostream>
#include <cassert>
#include <regex>

#include "common/source/letter.h"
#include "common/source/date.h"
#include "testing_utility.h"


namespace {
    void TestDate() {
        Date date = {3, 2, 2004};

        assert(date.GetDay() == 3);
        assert(date.GetMonth() == 2);
        assert(date.GetYear() == 2004);

        assert(date.AsString() == "03.02.2004");

        // Let assume this code won't be executed in the past:)
        assert(date < Date::CurrentDate());

        Date date2 = Date::ParseFrom("30.03.2003");

        assert(date2 == Date(30, 3, 2003));
        assert(date > date2);
        assert(date != date2);
        assert(date >= date2);
        assert(date2 <= date);

        ASSERT_THROWS(Date::ParseFrom("31.11.2022"), DateError); // There are only 30 days in november
        ASSERT_THROWS(Date(29, 2, 2022), DateError); // 2022 isn't leap year
        ASSERT_THROWS(Date(29, 2, 1900), DateError); // 1900 isn't leap year too (it divides by 100 and not 400)

        ASSERT_THROWS(Date(1, 1, -1), DateError); // Dates BC aren't allowed

        assert(Date(29, 2, 2020) == Date::ParseFrom("29.02.2020")); // OK, 2020 - leap year

        assert(std::regex_match("20.12.1981", std::regex{Date::regex}));
        assert(!std::regex_match("20/12/1981xxx", std::regex{Date::regex})); // '/' - separator isn't allowed
    }

    void TestLetterComparison() {
        Letter l = {"A", "B", "C", {20, 02, 2003}};
        Letter l2 = {"A", "B", "C", Date::ParseFrom("20.02.002003")};
        Letter l3 = {"X", "Y", "Z", {03, 07, 1983}};

        assert(l == l2);
        assert(!(l == l3));
    }
}


void TestLetter() {
    TestDate();
    TestLetterComparison();
    std::cerr << "TestLetter: OK" << std::endl;
}
