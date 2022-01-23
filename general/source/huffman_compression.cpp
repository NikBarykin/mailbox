#include "huffman_compression.h"

#include <memory>

//
//namespace {
//    struct Node {
//        size_t weight;
//        virtual std::string Serialize() = 0;
//
//    };
//
//    struct TransitNode : public Node {
//        std::unique_ptr<Node> left, right;
//    };
//
//    struct LeafNode : public Node {
//        char symbol;
//    };
//
//    std::unique_ptr<Node> BuildTree(std::vector<std::pair<char, size_t>> frequencies) {
//
//    }
//}

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
