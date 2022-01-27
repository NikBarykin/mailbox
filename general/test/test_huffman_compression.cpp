#include "test_huffman_compression.h"
#include "general/source/huffman_compression.h"

#include <iostream>
#include <random>
#include <string>
#include <cassert>
#include <numeric>


namespace {
    void CheckCompressionAndDecompression(const std::string & str) {
        assert(PerformHuffmanDecompression(PerformHuffmanCompression(str)) == str);
    }

    std::string CompressAndDecompress(const std::string & str) {
        return PerformHuffmanDecompression(PerformHuffmanCompression(str));
    }

    void TestGeneral() {
        std::mt19937 rnd(1234);
        std::string source_str(1000, 0);
        for (char & ch : source_str) {
            ch = rnd() % (1 << 7);
        }
        CheckCompressionAndDecompression(source_str);
        assert(CompressAndDecompress(source_str) == source_str);
    }

    void TestExtreme() {
        CheckCompressionAndDecompression("");

        // One unique character
        CheckCompressionAndDecompression(std::string(1000, 'x'));

        std::string every_character(1 << 7, 0);
        std::iota(every_character.begin(), every_character.end(), 0);
        CheckCompressionAndDecompression(every_character);
    }
}


void TestHuffmanCompression() {
    TestGeneral();
    TestExtreme();
    std::cerr << "TestHuffmanCompression: OK" << std::endl;
}
