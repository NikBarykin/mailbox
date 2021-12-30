#pragma once

#include "../../query.h"

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
    const std::unordered_map<std::string, ReadQueryPtr> read_query_funcs = {
            {"GetMail", &QueryReader::ReadGetMail}, {"SendLetter", &QueryReader::ReadSendLetter}
    };
public:
    QueryReader(std::istream& input, std::ostream& output);
    Query::QType operator ()();
};

Query::QType ReadGetMail(std::istream& input, std::ostream& output);
Query::QType ReadSendLetter(std::istream& input, std::ostream& output);
Query::QType ReadAuthorize(std::istream& input, std::ostream& output);
Query::QType ReadTerminate(std::istream& input, std::ostream& output);


Query::QType ReadQuery(std::istream& input);
