#include "protocol.h"
#include "utils.h"


#include <sstream>
#include <stdexcept>


// TODO: discard repeating strings like "output << EncodeSpecialSymbols(str) << "\n""
namespace Protocol {
    namespace {
        std::string SerializeTokens(const std::vector<std::string> & tokens) {
            std::ostringstream output;
            for (const std::string & token : tokens) {
                output << token.size() << "\n" << token;
            }
            return output.str();
        }

        std::vector<std::string> DeserializeTokens(const std::string & serialized_tokens) {
            std::istringstream input(serialized_tokens);
            std::vector<std::string> tokens;
            for (size_t token_sz; input >> token_sz; ) {
                input.ignore();
                tokens.emplace_back(token_sz, 0);
                for (char & token_ch : tokens.back()) {
                    input >> token_ch;
                }
            }
            return tokens;
        }
    }


//    Query::Query(std::string name): name(name) {}

    std::string Query::Serialize() const {
        std::vector<std::string> tokens(1 + arguments.size());
        tokens[0] = name;
        std::copy(arguments.begin(), arguments.end(), tokens.begin() + 1);
        return SerializeTokens(tokens);
    }

    Query Query::Deserialize(std::string serialized_query) {
        std::vector<std::string> tokens = DeserializeTokens(serialized_query);
        if (tokens.empty()) {
            throw std::runtime_error("Serialized query is empty");
        }
        return {tokens.front(), {tokens.begin() + 1, tokens.end()}};
    }

    std::string Answer::Serialize() const {
        std::vector<std::string> tokens(1 + fields.size());
        tokens[0] = error_message;
        std::copy(fields.begin(), fields.end(), tokens.begin() + 1);
        return SerializeTokens(tokens);
    }

    Answer Answer::Deserialize(std::string serialized_answer) {
        std::vector<std::string> tokens = DeserializeTokens(serialized_answer);
        if (tokens.empty()) {
            throw std::runtime_error("Serialized query is empty");
        }
        return {tokens.front(), {tokens.begin() + 1, tokens.end()}};
    }
}
