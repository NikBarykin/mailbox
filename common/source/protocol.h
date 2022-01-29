#pragma once

#include <string>
#include <vector>


namespace Protocol {
    // TODO: elegant constructors for different usage cases (for example, if query doesn't have arguments)
    struct Query {
        std::string name;
        std::vector<std::string> arguments;

        std::string Serialize() const;
        static Query Deserialize(std::string);
    };

    struct Answer {
        std::string error_message;
        std::vector<std::string> fields;

        std::string Serialize() const;
        static Answer Deserialize(std::string);
    };
}
