//
// Created by baryk on 28.12.2021.
//

#include "read_query.h"
#pragma once

#include "../query.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <optional>


Query::QType ReadAuthorize(std::istream& input) {
    output << "login:" << std::endl;

}


std::optional<Query::QType> ReadQuery(std::istream& input) {
    using ReadQueryPtr = Query::QType (*)(std::istream&);
    const std::unordered_map<std::string, ReadQueryPtr> read_query_funcs = {
            {"GetMail", ReadGetMail}, {"SendLetter", ReadSendLetter}
    };
    std::string query_name;
    std::getline(input, query_name);
    if (!read_query_funcs.count(query_name)) {
        return std::nullopt;
    }
    return read_query_funcs.at(query_name)(input);
}
