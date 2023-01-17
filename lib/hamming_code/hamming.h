#ifndef LAB4_HAMMING_H
#define LAB4_HAMMING_H

#include <iostream>
#include <vector>
#include <cmath>

uint64_t FindParity(uint64_t message_size);

uint64_t FindHammingLength(uint64_t message_size);

bool PowOfTwoCheck(uint64_t number);

bool CheckBitInNumber(uint64_t number, uint64_t bit_number, bool expected_value);

std::vector<bool> GenerateHammingCode(const std::vector<bool>& bits);

void PrintBits(const std::vector<bool>& bits);

uint64_t ToDecimal(uint64_t bits);

uint8_t ToDecimal(const std::vector<bool>& bits);

std::pair<uint64_t, bool> FindError(const std::vector<bool>& bits);

std::vector<bool> GetEncode(const std::vector<bool>& bits);

std::vector<bool> Decode(std::vector<bool>& bits);

#endif // LAB4_HAMMING_H
