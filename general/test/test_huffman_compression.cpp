#include "test_huffman_compression.h"
#include "general/source/huffman_compression.h"

#include <iostream>
#include <random>
#include <string>
#include <cassert>


namespace {
    void TestAll() {
        std::mt19937 rnd(1234);
        std::string source_str(1000, 0);
        for (char & ch : source_str) {
            ch = rnd() % (1 << 7);
        }
        std::string compressed_str = PerformHuffmanCompression(source_str);
        std::string decompressed_str = PerformHuffmanDecompression(compressed_str);

        assert(source_str == decompressed_str);
    }
}


void TestHuffmanCompression() {
    TestAll();
    std::cerr << "TestHuffmanCompression: OK" << std::endl;
}
