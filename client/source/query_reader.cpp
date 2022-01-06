#include "query_reader.h"
#include "general/source/utils.h"

#include <iostream>
#include <stdexcept>


QueryReader::QueryReader(const SessionState &session_state,
                         std::istream &input, std::ostream &output)
                         : session_state_(session_state), input_(input), output_(output) {}


Query::Any QueryReader::ReadGetMail() {
    return Query::GetMail{};
}

// TODO: maybe throw error if user didn't authorize
Query::Any QueryReader::ReadSendLetter() {
    Query::SendLetter query;

    query.letter.from = session_state_.user_login;

    output_ << "Letter recipient:" << std::endl;
    std::getline(input_, query.letter.to);

    output_ << "Letter body:" << std::endl;
    bool first_line = true;
    for (std::string letter_line; std::getline(input_, letter_line); ) {
        if (letter_line == "\\End-of-letter") {
            break;
        }
        if (first_line) {
            first_line = false;
        } else {
            query.letter.body += "\n";
        }
        query.letter.body += letter_line;
    }

    return query;
}

Query::Any QueryReader::ReadAuthorize() {
    Query::Authorize query;

    output_ << "Login:" << std::endl;
    std::getline(input_, query.login);

    output_ << "Password:" << std::endl;
    std::getline(input_, query.password);

    return query;
}

Query::Any QueryReader::ReadTerminate() {
    return Query::Terminate{};
}


Query::Any QueryReader::operator()() {
    output_ << "Query name:" << std::endl;
    std::string query_name;
    if (!std::getline(input_, query_name)) {
        throw std::runtime_error("Failed to read query name");
    }

    typedef Query::Any (QueryReader::*ReadQueryPtr)();
    static const std::unordered_map<std::string, ReadQueryPtr> read_query_methods {
            {Query::GetMail::name, &QueryReader::ReadGetMail},
            {Query::SendLetter::name, &QueryReader::ReadSendLetter},
            {Query::Authorize::name, &QueryReader::ReadAuthorize},
            {Query::Terminate::name, &QueryReader::ReadTerminate},
    };

    if (!read_query_methods.count(query_name)) {
        throw std::runtime_error("Invalid query name: " + query_name);
    }

    auto read_query_ptr = read_query_methods.at(query_name);
    Query::Any read_query = ((*this).*read_query_ptr)();
    output_ << std::endl;
    return read_query;
}
