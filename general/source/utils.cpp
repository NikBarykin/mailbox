#include "utils.h"


bool Replace(std::string& source, const std::string& from, const std::string& to) {
    size_t pos = source.find(from);
    if (pos == std::string::npos) {
        return false;
    }
    source.replace(pos, from.size(), to);
    return true;
}

void ReplaceAll(std::string& source, const std::string& from, const std::string& to) {
    while (Replace(source, from, to)) {}
}
