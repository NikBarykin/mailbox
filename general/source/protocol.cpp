#include "protocol.h"
#include "utils.h"


#include <sstream>
#include <stdexcept>


namespace Protocol {
    namespace {
        std::string EncodeSpecialSymbols(std::string str) {
            ReplaceAll(str, "\n", "\\n");
            return str;
        }

        std::string DecodeSpecialSymbols(std::string str) {
            ReplaceAll(str, "\\n", "\n");
            return str;
        }
    }


//    Query::Query(std::string name): name(name) {}

    std::string Query::Serialize() const {
        std::ostringstream oss;
        oss << EncodeSpecialSymbols(name);
        for (const std::string& argument : arguments) {
            oss << "\n" << EncodeSpecialSymbols(argument);
        }
        return oss.str();
    }

    Query Query::Deserialize(std::string serialized_query) {
        if (serialized_query.empty()) {
            throw std::runtime_error("Serialized query is empty");
        }
        std::istringstream iss(serialized_query);
        std::string query_name;
        std::getline(iss, query_name);
        query_name = DecodeSpecialSymbols(query_name);
        std::vector<std::string> query_args;
        for (std::string line; std::getline(iss, line); ) {
            query_args.push_back(DecodeSpecialSymbols(line));
        }
        return {query_name, query_args};
    }


//    Answer::Answer(std::string error_message, std::string body)
//    : error_message(error_message), body(body) {}

    std::string Answer::Serialize() const {
        std::ostringstream oss;
        oss << EncodeSpecialSymbols(error_message) << "\n";
        oss << EncodeSpecialSymbols(body);
        return oss.str();
    }

    Answer Answer::Deserialize(std::string serialized_answer) {
        std::istringstream iss(serialized_answer);
        std::string error_message;
        std::getline(iss, error_message);
        std::string body;
        std::getline(iss, body);
        return {DecodeSpecialSymbols(error_message), DecodeSpecialSymbols(body)};
    }
}
