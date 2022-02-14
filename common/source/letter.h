#pragma once

#include <string>

#include "date.h"


struct Letter {
    std::string from, to, body;
    Date date;
};


bool operator ==(const Letter& lhs, const Letter& rhs);
