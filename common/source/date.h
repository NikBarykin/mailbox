#pragma once

#include <iostream>
#include <stdexcept>


struct DateError : public std::runtime_error {
    explicit DateError(const std::string &msg)
             : std::runtime_error(msg) {}
};
// TODO:
class Date {
    int day_, month_, year_;
public:
    static constexpr char regex[] = R"(\d{1,2}\.\d{1,2}\.\d{1,})";

    Date(int day, int month, int year);

    int GetDay() const { return day_; };
    int GetMonth() const { return month_; };
    int GetYear() const { return year_; };

    std::string AsString() const;

    static Date CurrentDate();
    static Date ParseFrom(std::string_view);
};


bool operator==(const Date &lhs, const Date &rhs);
bool operator!=(const Date &lhs, const Date &rhs);
bool operator<(const Date &lhs, const Date &rhs);
bool operator>(const Date &lhs, const Date &rhs);
bool operator<=(const Date &lhs, const Date &rhs);
bool operator>=(const Date &lhs, const Date &rhs);
