#pragma once

#include "../query.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <optional>


Query::QType ReadGetMail(std::istream& input = std::cin) {

}


std::optional<Query::QType> ReadQuery(std::istream& input = std::cin) {
    using ReadQueryPtr = Query::QType (*)(std::istream&);
    const std::unordered_map<std::string, ReadQueryPtr> read_query_funcs = {
            {"GetMail", ReadGetMail}
    };
    std::string query_name;
    std::getline(input, query_name);
    if (!read_query_funcs.count(query_name)) {
        return std::nullopt;
    }
    return read_query_funcs.at(query_name)(input);
}
