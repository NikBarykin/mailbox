#include "protocol.h"
#include "utils.h"


#include <sstream>
#include <stdexcept>


// TODO: discard repeating strings like "output << EncodeSpecialSymbols(str) << "\n""
namespace Protocol {
    namespace {
        std::string EncodeSpecialSymbols(std::string str) {
            str = Replace(str, "\\", "\\\\");
            str = Replace(str, "\n", "\\n");
            return str;
        }

        std::string DecodeSpecialSymbols(std::string str) {
            str = Replace(str, "\\\\", "\\");
            str = Replace(str, "\\n", "\n");
            return str;
        }
    }


//    Query::Query(std::string name): name(name) {}

    std::string Query::Serialize() const {
        std::ostringstream oss;
        oss << EncodeSpecialSymbols(name) << "\n";
        for (const std::string& argument : arguments) {
            oss << EncodeSpecialSymbols(argument) << "\n";
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
        for (const std::string& field : fields) {
            oss << EncodeSpecialSymbols(field) << "\n";
        }
        return oss.str();
    }

    Answer Answer::Deserialize(std::string serialized_answer) {
        std::istringstream iss(serialized_answer);
        std::string error_message;
        std::getline(iss, error_message);
        error_message = DecodeSpecialSymbols(error_message);
        std::vector<std::string> fields;
        for (std::string field; std::getline(iss, field); ) {
            fields.push_back(DecodeSpecialSymbols(field));
        }
        return {error_message, fields};
    }
}
