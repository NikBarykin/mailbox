#include "huffman_compression.h"

#include <memory>


namespace {
    using CharConversion = std::vector<std::pair<char, std::vector<bool>>>;

    class Node {
    public:
        virtual CharConversion BuildConversion() const = 0;
        virtual std::string Serialize() const = 0;
    };

    class LeafNode : public Node{
    private:
        char ch;
    public:
        virtual CharConversion BuildConversion() const override;
        virtual std::string Serialize() const override;
    };

    class TransitNode : public Node {
    private:
        std::unique_ptr<Node> left_child, right_child ;
    public:
        TransitNode()

        virtual CharConversion BuildConversion() const override;
        virtual std::string Serialize() const override;
    };
}

std::string PerformHuffmanCompression(const std::string & str) {
    // TODO: implement
    // 1. Clc frequencies
    // 2. Build tree
    // 3. Serialize str using tree
    // 4. Serialize tree
    // First byte codes number of significant bits in the trailing byte
    return str;
}

std::string PerformHuffmanDecompression(const std::string & compressed_str) {
    // TODO: implement
    // 1. Deserialize tree
    // 2. Deserialize string using tree
    return compressed_str;
}
