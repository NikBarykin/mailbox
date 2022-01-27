#include "huffman_compression.h"

#include <memory>
#include <unordered_map>
#include <set>
#include <queue>
#include <forward_list>
#include <list>
#include <stdexcept>
#include <cassert>


// TODO: prettify
namespace {
    using CompressionRule = std::unordered_map<char, std::list<bool>>;
    constexpr size_t CHAR_BIT_SIZE = sizeof(char) * CHAR_BIT;

    // maybe:
    // using BitList = std::list<bool>;

    bool ExtractFirstBit(std::list<bool> & bits) {
        bool first_bit = bits.front();
        bits.pop_front();
        return first_bit;
    }

    std::list<bool> CharToBits(char ch) {
        std::list<bool> bits;
        for (size_t i = 0; i < CHAR_BIT_SIZE; ++i) {
            bits.push_back((ch >> i) & 1);
        }
        return bits;
    }

    class Node {
    public:
        virtual CompressionRule BuildCompressionRule() const = 0;
        // TODO: maybe ExtractCompressedChar
        virtual char ExtractAndDecompressChar(std::list<bool> & compressed_str) const = 0;
        virtual std::list<bool> Compress() const = 0;
    };

    class LeafNode : public Node{
    private:
        char ch;
    public:
        LeafNode(char ch): ch(ch) {}

        virtual CompressionRule BuildCompressionRule() const override {
            return {{ch, {}}};
        }

        virtual char ExtractAndDecompressChar(std::list<bool> & compressed_str) const override {
            return ch;
        }

        virtual std::list<bool> Compress() const override {
            std::list<bool> compressed = {true};
            compressed.splice(compressed.end(), CharToBits(ch));
            return compressed;
        }
    };

    class TransitNode : public Node {
    private:
        std::unique_ptr<Node> left_child, right_child;
    public:
        TransitNode(std::unique_ptr<Node> left_child, std::unique_ptr<Node> right_child)
        : left_child(std::move(left_child)), right_child(std::move(right_child)) {}

        virtual CompressionRule BuildCompressionRule() const override {
            CompressionRule l_rule = left_child->BuildCompressionRule();
            CompressionRule r_rule = right_child->BuildCompressionRule();

            for (auto & [ch, code] : l_rule) {
                code.push_front(false);
            }

            for (auto & [ch, code] : r_rule) {
                code.push_front(true);
            }

            l_rule.merge(r_rule);
            return l_rule;
        }

        virtual char ExtractAndDecompressChar(std::list<bool> & bits) const override {
            bool first_bit = ExtractFirstBit(bits);
            return (first_bit ? right_child : left_child)->ExtractAndDecompressChar(bits);
        }

        virtual std::list<bool> Compress() const override {
            std::list<bool> compressed = {false};
            compressed.splice(compressed.end(), left_child->Compress());
            compressed.splice(compressed.end(), right_child->Compress());
            return compressed;
        }
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

            auto [weight1, node_ptr1] = pop_begin(weighted_nodes);
            auto [weight2, node_ptr2] = pop_begin(weighted_nodes);

            auto new_node_ptr = std::make_unique<TransitNode>(
                    std::move(node_ptr1), std::move(node_ptr2));
            size_t new_weight = weight1 + weight2;

            weighted_nodes.emplace(new_weight, std::move(new_node_ptr));
        }
        return pop_begin(weighted_nodes).second;
    }

//    std::unordered_map<char, size_t> ComputeCharFrequencies(const std::string & str) {
//        std::unordered_map<char, size_t> frequencies;
//        for (char ch : str) {
//            ++frequencies[ch];
//        }
//        return frequencies;
//    }
//
//    std::list<bool> CompressString(const std::string & str, const CharMapping & compression_rule) {
//        std::list<bool> conversed;
//        conversed.
//
//
//        return conversed;
//    }
//
//    std::string DecompressString(const std::list<bool> & compressed_str, const CharMapping & compression_rule) {
//

    char ReadCharFromBits(std::list<bool> & bits) {
        if (bits.size() < CHAR_BIT_SIZE) {
            throw std::runtime_error("Too few bits (" + std::to_string(bits.size()) + ") to form a char");
        }
        char extracted_char = 0;
        for (size_t i = 0; i < CHAR_BIT_SIZE; ++i) {
            extracted_char |= ExtractFirstBit(bits) << i;
        }
        return extracted_char;
    }

    std::unique_ptr<Node> DecompressTree(std::list<bool> & bits) {
        if (bits.empty()) {
            throw std::runtime_error("Failed to decompress tree, not enough bits");
        }
        if (ExtractFirstBit(bits)) {
            char ch = ReadCharFromBits(bits);
            return std::make_unique<LeafNode>(ch);
        } else {
            auto left_tree = DecompressTree(bits);
            auto right_tree = DecompressTree(bits);
            return std::make_unique<TransitNode>(
                    std::move(left_tree), std::move(right_tree));
        }
    }

    std::string BitsToString(std::list<bool> bits) {
        char n_insignificant_bits = (CHAR_BIT_SIZE - bits.size() % CHAR_BIT_SIZE) % CHAR_BIT_SIZE;
        bits.splice(bits.end(), std::list<bool>(n_insignificant_bits, false));
        // TODO: optimize by passing resulting size to constructor
        std::string resulting_str = {n_insignificant_bits};

        while (!bits.empty()) {
            resulting_str.push_back(ReadCharFromBits(bits));
        }

        return resulting_str;
    }

    std::list<bool> StringToBits(std::string_view bits_as_str_view) {
        char n_insignificant_bits = bits_as_str_view.front();
        bits_as_str_view.remove_prefix(1);
        std::list<bool> bits;
        while (!bits_as_str_view.empty()) {
            bits.splice(bits.end(), CharToBits(bits_as_str_view.front()));
            bits_as_str_view.remove_prefix(1);
        }
        bits.resize(bits.size() - n_insignificant_bits);
        return bits;
    }
}

std::string PerformHuffmanCompression(const std::string & str) {
    // TODO: process string containing only one unique character
    // 1. Clc frequencies
    // 2. Build tree
    // 3. Serialize str using tree
    // 4. Serialize tree
    // First byte codes number of significant bits in the trailing byte

    std::unordered_map<char, size_t> char_frequencies;
    for (char ch : str) {
        ++char_frequencies[ch];
    }

    std::unique_ptr<Node> tree = BuildTree(char_frequencies);
    std::list<bool> resulting_compression = tree->Compress();
    CompressionRule rule = tree->BuildCompressionRule();
    for (char ch : str) {
        std::list<bool> ch_compression = rule[ch];
        resulting_compression.splice(resulting_compression.end(), std::move(ch_compression));
    }

    return BitsToString(resulting_compression);
}

std::string PerformHuffmanDecompression(const std::string & compressed_str) {
    std::list<bool> bit_compression = StringToBits(compressed_str);
    std::unique_ptr<Node> tree = DecompressTree(bit_compression);
    std::string decompressed_str;
    while (!bit_compression.empty()) {
        decompressed_str.push_back(tree->ExtractAndDecompressChar(bit_compression));
    }
    return decompressed_str;
}
