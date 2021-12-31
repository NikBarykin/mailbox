#pragma once

#include "general/source/query.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <optional>


class QueryReader {
private:
    std::istream& input_;
    std::ostream& output_;

    Query::QType ReadGetMail();
    Query::QType ReadSendLetter();
    Query::QType ReadAuthorize();
    Query::QType ReadTerminate();

    typedef Query::QType (QueryReader::*ReadQueryPtr)();
    const std::unordered_map<std::string, ReadQueryPtr> read_query_methods = {
            {"GetMail", &QueryReader::ReadGetMail}, {"SendLetter", &QueryReader::ReadSendLetter}
    };
public:
    QueryReader(std::istream& input, std::ostream& output);
    Query::QType operator ()();
};
