#include "date.h"


#include <tuple>
#include <sstream>
#include <ctime>
#include <iomanip>


Date::Date(int day, int month, int year):
day_(day), month_(month), year_(year) {
    if (year < 0) { // TODO: idk maybe <=
        throw DateError("Bad year: " + std::to_string(year) + " (Years BC aren't allowed)");
    }

    if (month <= 0 || month > 12) {
        throw DateError("Bad month: " + std::to_string(month));
    }

    bool leap_year = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    int month_durations[12] = {31, 28 + leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day <= 0 || day > month_durations[month - 1]) {
        throw DateError("Bad day: " + std::to_string(day));
    }
}

std::string Date::AsString() const {
    std::ostringstream oss;
    oss << std::setfill('0');
    oss << std::setw(2) << day_ << '.' << std::setw(2) << month_ << '.' << year_;
    return oss.str();
}

Date Date::CurrentDate() {
    time_t now = time(nullptr);
    std::tm* moment = gmtime(&now);
    return {moment->tm_mday, moment->tm_mon + 1, moment->tm_year + 1900};
}

Date Date::ParseFrom(std::string_view date_str) {
    std::istringstream iss(std::string{date_str});
    int day, month, year;
    bool ok = true;
    iss >> day;
    ok &= iss.peek() == '.';
    iss.ignore();
    iss >> month;
    ok &= iss.peek() == '.';
    iss.ignore();
    iss >> year;
    ok &= !iss.fail() && iss.eof();
    if (!ok) {
        throw DateError("Failed to parse date from string: " + std::string{date_str});
    }
    return {day, month, year};
}


bool operator<(const Date &lhs, const Date &rhs) {
    return  std::make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
            std::make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

bool operator==(const Date &lhs, const Date &rhs) {
    return !(lhs < rhs) && !(rhs < lhs);
}

bool operator!=(const Date &lhs, const Date &rhs) {
    return !(lhs == rhs);
}

bool operator>(const Date &lhs, const Date &rhs) {
    return rhs < lhs;
}

bool operator<=(const Date &lhs, const Date &rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator>=(const Date &lhs, const Date &rhs) {
    return lhs > rhs || lhs == rhs;
}
