#pragma once

#include <iostream>


// TODO:
class Date {
    int day_, month_, year_;
public:
    Date(int day, int month, int year);
    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;

    static Date CurrentDate();
    static Date ParseFrom(std::istream & input);
};

std::ostream &operator<<(std::ostream & output, const Date & d);

bool operator==(const Date &lhs, const Date &rhs);
bool operator!=(const Date &lhs, const Date &rhs);
bool operator<(const Date &lhs, const Date &rhs);
bool operator>(const Date &lhs, const Date &rhs);
bool operator<=(const Date &lhs, const Date &rhs);
bool operator>=(const Date &lhs, const Date &rhs);
