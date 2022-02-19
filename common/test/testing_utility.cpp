#include "testing_utility.h"

#include <sstream>


Letter operator "" _L(const char* letter_str, size_t) {
    std::istringstream iss(letter_str);
    Letter result;
    std::getline(iss, result.from, ' ');
    std::getline(iss, result.to, ' ');
    std::getline(iss, result.body);
    return result;
}
