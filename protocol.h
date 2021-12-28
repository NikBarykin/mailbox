#pragma once

#include <string>
#include <vector>


namespace Protocol {
    std::string SerializeQuery(std::string query_name, std::vector<std::string> arguments);
    std::pair<std::string, std::vector<std::string>> DeserializeQuery(std::string);
    std::string SerializeAnswer(std::string error_message, std::string body);
    std::pair<std::string, std::string> DeserializeAnswer(std::string);
}