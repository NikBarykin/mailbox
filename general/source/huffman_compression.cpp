#include "huffman_compression.h"

#include <memory>
#include <unordered_map>
#include <set>
#include <queue>


namespace {
//    using CompressionRule = std::unordered_map<char, std::vector<bool>>;
    using CompressionRule = std::unordered_map<char, std::vector<bool>>;

    class Node {
    public:
        virtual CharConversion BuildConversion() const = 0;
        virtual char DecompressFirstChar(std::queue<bool> & compressed_str) const = 0;
        virtual std::vector<bool> Serialize() const = 0;
    };

    class LeafNode : public Node{
    private:
        char ch;
    public:
        LeafNode(char ch): ch(ch) {}

        virtual CharConversion BuildConversion() const override {
            return {{ch, {}}};
        }

        virtual std::vector<bool> Serialize() const override {
        }
    };

    class TransitNode : public Node {
    private:
        std::unique_ptr<Node> left_child, right_child;
    public:
        TransitNode(std::unique_ptr<Node> left_child, std::unique_ptr<Node> right_child)
        : left_child(std::move(left_child)), right_child(std::move(right_child)) {}

        virtual CharConversion BuildConversion() const override {
            CharConversion l_conversion = left_child->BuildConversion();
            CharConversion r_conversion = right_child->BuildConversion();

            for (auto & [ch, code] : l_conversion) {
                code.push_back(false);
            }

            for (auto & [ch, code] : r_conversion) {
                code.push_back(true);
            }

            l_conversion.merge(r_conversion);
            return l_conversion;
        }

        virtual std::vector<bool> Serialize() const override;
    };


    std::unique_ptr<Node> BuildTree(const std::unordered_map<char, size_t> & initial_weights) {
        std::set<std::pair<size_t, std::unique_ptr<Node>>> weighted_nodes;
        for (const auto & [ch, weight] : initial_weights) {
            weighted_nodes.emplace(weight, std::make_unique<LeafNode>(ch));
        }

        auto pop_begin = []<class T>(std::set<T>& s) -> T {
            return std::move(s.extract(s.begin()).value());
        };

        while (weighted_nodes.size() > 1) {
//            auto [weight1, node_ptr1] = std::move(
//                    weighted_nodes.extract(weighted_nodes.begin()).value());
//            auto [weight2, node_ptr2] = std::move(
//                    weighted_nodes.extract(weighted_nodes.begin()).value());
//
            auto [weight1, node_ptr1] = pop_begin(weighted_nodes);
            auto [weight2, node_ptr2] = pop_begin(weighted_nodes);

            auto new_node_ptr = std::make_unique<TransitNode>(
                    std::move(node_ptr1), std::move(node_ptr2));
            size_t new_weight = weight1 + weight2;

            weighted_nodes.emplace(new_weight, std::move(new_node_ptr));
        }
        return pop_begin(weighted_nodes).second;
    }

    std::unordered_map<char, size_t> ComputeCharFrequencies(const std::string & str) {
        std::unordered_map<char, size_t> frequencies;
        for (char ch : str) {
            ++frequencies[ch];
        }
        return frequencies;
    }

    std::vector<bool> CompressString(const std::string & str, const CharMapping & compression_rule) {
        std::vector<bool> conversed;
        conversed.


        return conversed;
    }
//
//    std::string DecompressString(const std::vector<bool> & compressed_str, const CharMapping & compression_rule) {
//
//    }

    std::string BitsToString(const std::vector<bool> & bits) {

    }

    char ReadCharFromBits(std::queue<bool> & bits) {

    }
}

std::string PerformHuffmanCompression(const std::string & str) {
    // TODO: process string containing only one unique character
    std::unordered_map<char, size_t> frequencies;
    for (char ch : str) {
        ++frequencies[ch];
    }

    std::unique_ptr<Node> tree = BuildTree(frequencies);
    std::vector<bool> compression_result = tree->Serialize();
    CompressionRule rule = tree->BuildCompressionRule();
    for (char ch : str) {
        std::vector<bool> ch_compression = rule[ch];
        compressed_str.insert(compressed_str.end(),
                              ch_compression.begin(), ch_compression.end());
    }

    return BitsToString(compressed_str);
    char n_insignificant_trailing_bits = (8 - compressed_str.size() % 8) % 8;
    compression_result.resize(compression_result.size() + n_insignificant_trailing_bits, false);
    while (!compression_result.empty()) {
        result.push_back(ReadCharFromBits(compression_result));
    }
    std::string result;
    return result;

//
//
//    auto frequencies = ComputeCharFrequencies(str);
//    auto tree = BuildTree(frequencies);
//    auto str_bits = ConverseString(str, tree->BuildConversion());
//    auto tree_bits = tree->Serialize();

    // TODO: implement
    // 1. Clc frequencies
    // 2. Build tree
    // 3. Serialize str using tree
    // 4. Serialize tree
    // First byte codes number of significant bits in the trailing byte
    std::queue<int> q;
    return str;
}

std::string PerformHuffmanDecompression(const std::string & compressed_str) {
    // TODO: implement
    std::vector<bool> bits = BitsFromString(compressed_str);
    char n_insignificant_trailing_bits = ReadCharFromBits(bits);
    bits.resize(bits.size() - n_insignificant_trailing_bits);

    auto tree = DeserializeTree(bits);
    std::string decompressed_str;
    while (!bits.empty()) {
        decompressed_str.push_back(tree->DecompressFirstChar(bits));
    }
    return decompressed_str;
    // 1. Deserialize tree
    // 2. Deserialize string using tree
    return compressed_str;
}
