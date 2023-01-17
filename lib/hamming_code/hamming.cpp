#include "hamming.h"

uint64_t FindParity(uint64_t message_size) {
    uint64_t parity = 0;

    while (static_cast<uint64_t>(std::pow(2, parity)) < parity + message_size + 1) {
        ++parity;
    }

    return parity;
}

uint64_t FindHammingLength(uint64_t message_size) {
    return FindParity(message_size) + message_size;
}

bool PowOfTwoCheck(uint64_t number) {
    long double logarithm = log2((double)number);
    return logarithm == static_cast<uint64_t>(logarithm);
}

bool XOR(const std::vector<bool>& bits) {
    bool xor_bit = bits[0];

    for (uint64_t index = 1; index < bits.size(); ++index)
        xor_bit ^= bits[index];

    return xor_bit;
}

bool CheckBitInNumber(uint64_t number, uint64_t bit_number, bool expected_value) {
    std::vector<bool> bits;

    while (number > 0) {
        bits.emplace_back(number % 2);
        number /= 2;
    }

    return bits[bit_number - 1] == expected_value;
}


std::vector<bool> GenerateHammingCode(const std::vector<bool>& bits) {
    uint64_t hamming_code_size = FindHammingLength(bits.size());
    uint64_t pointer = bits.size() - 1;
    std::vector<bool> hamming_code(hamming_code_size, false);

    for (uint64_t index = 0; index < hamming_code_size; ++index) {
        if (!PowOfTwoCheck(index + 1)) {
            hamming_code[index] = bits[pointer];
            --pointer;
        }
    }

    uint64_t pow_of_two = 1;
    uint64_t bit_number = 1;

    while (pow_of_two <= hamming_code_size) {
        uint64_t tmp = 0;

        for (uint64_t index = pow_of_two; index < hamming_code_size; ++index) {
            if (CheckBitInNumber(index + 1, bit_number, true)) {
                tmp += hamming_code[index];
            }
        }

        hamming_code[pow_of_two - 1] = tmp % 2;

        ++bit_number;
        pow_of_two *= 2;
    }

    hamming_code.push_back(XOR(hamming_code));

    return std::move(hamming_code);
}

void PrintBits(const std::vector<bool> &bits) {
    for (long long i = (long long)bits.size() - 1; i >= 0; --i)
        std::cout << bits[i];
    std::cout << std::endl;
}

uint64_t ToDecimal(uint64_t bits) {
    uint64_t result = 0;
    uint64_t power = 0;

    while (bits > 0) {
        result += static_cast<uint64_t>((double)(bits % 10) * pow(2, (double)power));
        ++power;
        bits /= 10;
    }

    return result;
}

uint8_t ToDecimal(const std::vector<bool>& bits) {
    uint8_t result = 0;

    for (uint64_t index = bits.size(); index > 0; --index) {
        result += (uint8_t)bits[index - 1] * (uint8_t)pow(2, (double)(bits.size() - index));
    }

    return result;
}

std::pair<uint64_t, bool> FindError(const std::vector<bool>& bits) {
    uint64_t pow_of_two = 1;
    uint64_t mistake_value = 0;

    // 1 xor bit in the end
    while (pow_of_two <= bits.size() - 1) {
        uint64_t tmp = 0;

        for (uint64_t j = bits.size() - 1; j >= pow_of_two; --j) {
            if ((j & pow_of_two) == pow_of_two) {
                tmp ^= bits[j - 1];
            }
        }

        if (tmp)
            mistake_value += static_cast<uint64_t>(pow(10, log2((double)pow_of_two)));
        pow_of_two *= 2;
    }

    bool p = XOR(bits);

    uint64_t mistake_decimal = ToDecimal(mistake_value);

    if (p == 0 && mistake_decimal != 0)
        throw std::runtime_error("Find 2 errors in word");

    return std::move(std::make_pair(mistake_decimal, p));
}

std::vector<bool> GetEncode(const std::vector<bool>& bits) {
    std::vector<bool> encode;

    for (uint64_t j = bits.size() - 1; j > 0; --j) {
        if (!PowOfTwoCheck(j))
            encode.emplace_back(bits[j - 1]);
    }

    return std::move(encode);
}

std::vector<bool> Decode(std::vector<bool>& bits) {
    std::pair<uint64_t, bool> error;

    try {
        error = FindError(bits);
        if (error.first > 0 && error.second)
            bits[error.first - 1] = !bits[error.first - 1];

    } catch (const std::runtime_error &err) {
        std::cerr << err.what();

        throw std::invalid_argument("Code can not be encoded. It has more than one error");
    }

    return std::move(GetEncode(bits));
}
