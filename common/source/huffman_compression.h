#pragma once

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>


namespace Huffman {
    struct Error : public std::runtime_error {
        explicit Error(const std::string &msg)
        : std::runtime_error(msg) {}
    };

    std::string PerformCompression(const std::string &);

    std::string PerformDecompression(const std::string &);
}
