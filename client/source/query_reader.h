#pragma once

#include "general/source/query.h"
#include "session.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <optional>


class QueryReader {
private:
    const SessionState& session_state_;
    std::istream& input_;
    std::ostream& output_;

public:
    QueryReader(const SessionState&, std::istream& input, std::ostream& output);

private:
    // These methods don't read query name, only it's arguments
    Query::Any ReadGetMail();
    Query::Any ReadSendLetter();
    Query::Any ReadAuthorize();
    Query::Any ReadTerminate();

public:
    Query::Any operator ()();
};
