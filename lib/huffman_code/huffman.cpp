#include "huffman.h"
#include <iostream>

std::map<uint16_t, uint64_t> CountSymbols(const std::string& path) {
    std::ifstream file;
    file.open(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Can not open file " + path);
    }

    std::map<uint16_t, uint64_t> count_symbols;

    while (!file.eof()) {
        int s = file.get();

        if (s == -1)
            break;

        if (count_symbols.contains(s)) {
            ++count_symbols[s];
        } else {
            count_symbols[s] = 1;
        }
    }

    return std::move(count_symbols);
}

void SortPairsDeq(auto& pairs) {
    std::sort(pairs.begin(), pairs.end(), [](auto first_item, auto second_item) {
        if (first_item.second < second_item.second)
            return true;

        return false;
    });
}


auto PairsToDeque(const std::map<uint16_t, uint64_t>& symbols) {
    std::deque<std::pair<std::set<uint16_t>, uint64_t>> pairs;

    for (const auto& item: symbols) {
        std::set<uint16_t> tmp = {item.first};
        pairs.emplace_back(std::make_pair(tmp, item.second));
    }

    SortPairsDeq(pairs);

    return std::move(pairs);
}

void CodeCollecting(
        std::map<uint16_t, std::string>& huffman,
        const std::pair<std::set<uint16_t>, uint64_t>& item1,
        const std::pair<std::set<uint16_t>, uint64_t>& item2) {
    for (const auto& itr: item1.first) {
        if (huffman.contains(itr)) {
            huffman[itr] += "0";
        } else {
            huffman[itr] = "0";
        }
    }

    for (const auto& itr: item2.first) {
        if (huffman.contains(itr)) {
            huffman[itr] += "1";
        } else {
            huffman[itr] = "1";
        }
    }
}

std::set<uint16_t> Merge(
        const std::set<uint16_t>& item1,
        const std::set<uint16_t>& item2) {
    std::set<uint16_t> merged;

    for (const auto& itr: item1)
        merged.insert(itr);

    for (const auto& itr: item2)
        merged.insert(itr);

    return std::move(merged);
}

std::map<uint16_t, std::string> GenerateHuffmanCode(const std::string& path) {

    std::map<uint16_t, uint64_t> count_symbols = CountSymbols(path);

    auto pairs = PairsToDeque(count_symbols);

    std::map<uint16_t, std::string> huffman_code;

    if (pairs.size() == 1) {
        huffman_code[*pairs.front().first.begin()] = "0";
        return huffman_code;
    }

    while (pairs.size() >= 2) {
        auto item1 = pairs.front();
        pairs.pop_front();
        auto item2 = pairs.front();
        pairs.pop_front();

        CodeCollecting(huffman_code, item1, item2);

        pairs.push_back(
                std::make_pair(Merge(item1.first, item2.first), item1.second + item2.second)
        );

        SortPairsDeq(pairs);
    }

    for (auto& item: huffman_code) {
        std::reverse(huffman_code[item.first].begin(), huffman_code[item.first].end());
    }

    return std::move(huffman_code);
}


std::pair<std::vector<uint8_t>, std::vector<bool>> EncodeToByte(const std::vector<bool>& data_to_decode,
                                                                std::map<std::string, uint16_t>& code) {
    std::vector<uint8_t> decoded_data;
    std::string current_sequence;

    for (const auto& current_bit: data_to_decode) {
        if (current_bit)
            current_sequence += "1";
        else
            current_sequence += "0";

        if (code.contains(current_sequence)) {
            decoded_data.push_back(code[current_sequence]);
            current_sequence.clear();
        }
    }

    if (code.contains(current_sequence)) {
       decoded_data.push_back(code[current_sequence]);
    }

    std::vector<bool> remain;
    for (const char& bit: current_sequence)
        remain.push_back(bit - '0');

    return std::make_pair(decoded_data, remain);
}