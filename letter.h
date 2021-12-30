#pragma once

#include <string>


struct Letter {
    std::string from, to, body;
};


bool operator ==(const Letter& lhs, const Letter& rhs);
