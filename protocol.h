#pragma once

#include <string>
#include <vector>


namespace Protocol {
    struct Query {
        std::string name_;
        std::vector<std::string> arguments_;

        Query(std::string name);

        template<class StringIt>
        Query(std::string name, StringIt arg_begin, StringIt arg_end);

        template<class StringContainer>
        Query(std::string name, StringContainer arguments);

        std::string Serialize() const;
        static Query Deserialize(std::string);
    };

    struct Answer {
        std::string error_message_;
        std::string body_;

        Answer(std::string error_message_, std::string body_);

        std::string Serialize() const;
        static Answer Deserialize(std::string);
    };
}


// Implementations

namespace Protocol {
    template<class StringIt>
    Query::Query(std::string name, StringIt arg_begin, StringIt arg_end)
    : name_(name), arguments_(arg_begin, arg_end) {}

    template<class StringContainer>
    Query::Query(std::string name, StringContainer arguments)
    : Query(name, std::begin(arguments), std::end(arguments)) {}
}
