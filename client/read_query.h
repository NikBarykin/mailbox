#pragma once

#include "../query.h"

#include <iostream>
#include <string>


Query::QType ReadQuery(std::istream& input = std::cin) {
    std::string query_name;
    std::getline(input, query_name);
    if (query_name == "GetMail") {
        
    }
}