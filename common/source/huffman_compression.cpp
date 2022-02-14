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

    bool ExtractFirstBit(std::list<bool> & bits) {
        bool first_bit = bits.front();
        bits.pop_front();
        return first_bit;
    }

    std::list<bool> CharToBits(char ch) {
        std::list<bool> bits;
        for (size_t i = 0; i < CHAR_BIT; ++i) {
            bits.push_back((ch >> i) & 1);
        }
        return bits;
    }

    class Node {
    public:
        virtual CompressionRule BuildCompressionRule() const = 0;
        virtual char ExtractCompressedChar(std::list<bool> & compressed_str) const = 0;
        virtual std::list<bool> Compress() const = 0;
    };

    class LeafNode : public Node{
    private:
        char ch;
    public:
        explicit LeafNode(char ch): ch(ch) {}

        CompressionRule BuildCompressionRule() const final {
            return {{ch, {}}};
        }

        char ExtractCompressedChar(std::list<bool> &) const final {
            return ch;
        }

        std::list<bool> Compress() const final {
            std::list<bool> compressed = {true};
            compressed.splice(compressed.end(), CharToBits(ch));
            return compressed;
        }
    };

    class TransitNode : public Node {
    private:
        std::shared_ptr<Node> left_child, right_child;
    public:
        TransitNode(std::shared_ptr<Node> left_child, std::shared_ptr<Node> right_child)
        : left_child(left_child), right_child(right_child) {}

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

        virtual char ExtractCompressedChar(std::list<bool> & bits) const override {
            bool first_bit = ExtractFirstBit(bits);
            return (first_bit ? right_child : left_child)->ExtractCompressedChar(bits);
        }

        virtual std::list<bool> Compress() const override {
            std::list<bool> compressed = {false};
            compressed.splice(compressed.end(), left_child->Compress());
            compressed.splice(compressed.end(), right_child->Compress());
            return compressed;
        }
    };

    std::shared_ptr<Node> BuildTree(const std::unordered_map<char, size_t> & initial_weights) {
        assert(initial_weights.size() >= 2);

        std::set<std::pair<size_t, std::shared_ptr<Node>>> weighted_nodes;
        for (const auto & [ch, weight] : initial_weights) {
            weighted_nodes.emplace(weight, std::make_shared<LeafNode>(ch));
        }

        auto pop_begin = []<class T>(std::set<T>& s) -> T {
            return s.extract(s.begin()).value();
        };

        while (weighted_nodes.size() > 1) {
            auto [weight_r, node_r] = pop_begin(weighted_nodes);
            auto [weight_l, node_l] = pop_begin(weighted_nodes);

            auto new_node = std::make_shared<TransitNode>(node_l, node_r);
            size_t new_weight = weight_l + weight_r;

            weighted_nodes.emplace(new_weight, new_node);
        }
        return pop_begin(weighted_nodes).second;
    }

    char ReadCharFromBits(std::list<bool> & bits) {
        if (bits.size() < CHAR_BIT) {
            throw std::runtime_error("Too few bits to form a char: " + std::to_string(bits.size()));
        }
        char extracted_char = 0;
        for (size_t i = 0; i < CHAR_BIT; ++i) {
            extracted_char |= ExtractFirstBit(bits) << i;
        }
        return extracted_char;
    }

    std::shared_ptr<Node> DecompressTree(std::list<bool> & bits) {
        if (bits.empty()) {
            throw std::runtime_error("Failed to decompress tree, not enough bits");
        }
        if (ExtractFirstBit(bits)) {
            char ch = ReadCharFromBits(bits);
            return std::make_shared<LeafNode>(ch);
        } else {
            auto left_subtree = DecompressTree(bits);
            auto right_subtree = DecompressTree(bits);
            return std::make_shared<TransitNode>(left_subtree, right_subtree);
        }
    }

    std::string BitsToString(std::list<bool> bits) {
        char n_insignificant_bits = (CHAR_BIT - bits.size() % CHAR_BIT) % CHAR_BIT;
        bits.splice(bits.end(), std::list<bool>(n_insignificant_bits, false));
        std::string resulting_str = {n_insignificant_bits};

        while (!bits.empty()) {
            resulting_str.push_back(ReadCharFromBits(bits));
        }

        return resulting_str;
    }

    std::list<bool> StringToBits(std::string_view bits_as_str) {
        char n_insignificant_bits = bits_as_str.front();
        bits_as_str.remove_prefix(1);
        std::list<bool> bits;
        while (!bits_as_str.empty()) {
            bits.splice(bits.end(), CharToBits(bits_as_str.front()));
            bits_as_str.remove_prefix(1);
        }
        bits.resize(bits.size() - n_insignificant_bits);
        return bits;
    }
}


namespace Huffman {
    std::string PerformCompression(const std::string &str) {
        std::unordered_map<char, size_t> frequencies;
        for (char ch: str) {
            ++frequencies[ch];
        }
        // frequencies size should be at least 2, otherwise we can't build a valid tree
        for (char ch = 0; frequencies.size() < 2; ++ch) frequencies.emplace(ch, 0);

        std::shared_ptr<Node> tree = BuildTree(frequencies);
        std::list<bool> resulting_compression = tree->Compress();
        CompressionRule rule = tree->BuildCompressionRule();
        for (char ch: str) {
            std::list<bool> compressed_ch = rule[ch];
            resulting_compression.splice(resulting_compression.end(), std::move(compressed_ch));
        }

        return BitsToString(resulting_compression);
    }

    std::string PerformDecompression(const std::string &compressed_str) {
        std::list<bool> bit_compression = StringToBits(compressed_str);
        std::shared_ptr<Node> tree = DecompressTree(bit_compression);
        std::string decompressed_str;
        while (!bit_compression.empty()) {
            decompressed_str.push_back(tree->ExtractCompressedChar(bit_compression));
        }
        return decompressed_str;
    }
}
