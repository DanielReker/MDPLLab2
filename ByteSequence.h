#pragma once

#include <format>
#include <regex>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <bitset>
#include <iomanip>
#include <concepts>
#include <sstream>

using ByteSequence = std::vector<std::byte>;

template<std::integral T>
ByteSequence encodeNumberLittleEndian(T number, int bytes) {
    ByteSequence result;
    for (int i = 0; i < bytes; i++) {
        result.push_back(static_cast<std::byte>(number));
        number >>= 8;
    }
    return result;
}

std::ostream& operator<<(std::ostream& out, std::byte byte) {
    out << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << std::to_integer<int>(byte);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ByteSequence& byteSequence) {
    for (auto byte : byteSequence) {
        out << byte << ' ';
    }
    return out;
}

std::string toString(const ByteSequence& byteSequence) {
    std::stringstream result;
    result << byteSequence;
    return result.str();
}
