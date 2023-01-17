#ifndef LAB4_HUFFMAN_H
#define LAB4_HUFFMAN_H

#include <fstream>
#include <map>
#include <deque>
#include <algorithm>
#include <set>

std::map<uint16_t, uint64_t> CountSymbols(std::ifstream& file);

void SortPairsDeq(auto& pairs);

auto PairsToDeque(const std::map<uint16_t, uint64_t>& symbols);

void CodeCollecting(
        std::map<uint16_t, std::string>& huffman,
        const std::pair<std::set<uint16_t>, uint64_t>& item1,
        const std::pair<std::set<uint16_t>, uint64_t>& item2);

std::set<uint16_t> Merge(
        const std::set<uint16_t>& item1,
        const std::set<uint16_t>& item2);

std::map<uint16_t, std::string> GenerateHuffmanCode(const std::string& path);

std::pair<std::vector<uint8_t>, std::vector<bool>> EncodeToByte(
        const std::vector<bool>& data_to_encode,
        std::map<std::string, uint16_t>& codes);

#endif //LAB4_HUFFMAN_H
