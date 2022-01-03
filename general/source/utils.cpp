#include "utils.h"

#include <string_view>


std::string Replace(const std::string& source, const std::string& from, const std::string& to) {
    std::string res_str;
    std::string_view sv = source;
    while (!sv.empty()) {
        size_t pos = sv.find(from);
        res_str += sv.substr(0, pos);
        if (pos == std::string::npos) {
            break;
        }
        sv.remove_prefix(pos + from.size());
        res_str += to;
    }
    return res_str;
}
