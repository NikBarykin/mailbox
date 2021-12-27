#pragma once

#include "../letter.h"


#include <vector>


class Database {
public:
    using LetterId = size_t;
private:
    std::vector<Letter> letters_;
};
