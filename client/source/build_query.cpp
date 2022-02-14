#include "build_query.h"
#include "common/source/utils.h"

#include <unordered_map>
#include <functional>
#include <map>


namespace {
    Query::Any BuildGetMail(std::istream & input, std::ostream & output,
                            const SessionState & session_state) {
        Query::GetMail query;
        output << "Letter filter:" << std::endl;
        std::getline(input, query.letter_filter);
        return query;
    }

    // TODO: maybe throw exception if user didn't authorize
    Query::Any BuildSendLetter(std::istream & input, std::ostream & output,
                               const SessionState & session_state) {
        Query::SendLetter query;

        query.letter.from = session_state.user_login;

        output << "Letter recipient:" << std::endl;
        std::getline(input, query.letter.to);

        output << "Letter body:" << std::endl;
        bool first_line = true;
        for (std::string letter_line; std::getline(input, letter_line); ) {
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

    Query::Any BuildAuthorize(std::istream & input, std::ostream & output,
                              const SessionState & session_state) {
        Query::Authorize query;

        output << "Login:" << std::endl;
        std::getline(input, query.login);

        output << "Password:" << std::endl;
        std::getline(input, query.password);

        return query;
    }

    Query::Any BuildTerminate(std::istream & input, std::ostream & output,
                              const SessionState & session_state) {
        return Query::Terminate{};
    }
}


Query::Any BuildQuery(std::istream & input, std::ostream & output,
                      const SessionState & session_state) {
    using QueryBuilder = std::function<Query::Any(
            std::istream &, std::ostream &, const SessionState &)>;
    // Not unordered_map because I want map keys to be in specified order
    static const std::map<std::string, QueryBuilder> query_builders{
            {Query::GetMail::name,    BuildGetMail},
            {Query::SendLetter::name, BuildSendLetter},
            {Query::Authorize::name,  BuildAuthorize},
            {Query::Terminate::name,  BuildTerminate},
    };

    std::string query_name;
    while (true) {
        output << "Query name:" << std::endl;
        if (!std::getline(input, query_name)) {
            throw std::runtime_error("Failed to read query name");
        }

        if (query_builders.count(query_name)) {
            break;
        }

        if (query_name == "Help") {
            output << "Valid queries: ";
            bool first = true;
            for (const auto &[valid_name, _] : query_builders) {
                if (first) {
                    first = false;
                } else {
                    output << ", ";
                }
                output << valid_name;
            }
            output << std::endl;
        } else {
            output << "Unknown query, type \"Help\" to get list of queries" << std::endl;
        }
    }

    auto query_builder = query_builders.at(query_name);
    Query::Any read_query = query_builder(input, output, session_state);
    output << std::endl;
    return read_query;
}
