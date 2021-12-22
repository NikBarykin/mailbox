#pragma once

#include <string>


struct Letter {
    std::string from, to, message;

    std::string Serialize() const;
    static Letter Deserialize(std::string);
};
