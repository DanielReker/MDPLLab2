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


ByteSequence encodeString(const std::string& string) {
    auto stringTransformed = string | std::views::transform([](char c) { return std::byte(c); });
    return ByteSequence(stringTransformed.begin(), stringTransformed.end());
}

template<std::integral T>
ByteSequence encodeNumberLittleEndian(T number, int bytes) {
    ByteSequence result;
    for (int i = 0; i < bytes; i++) {
        result.push_back(static_cast<std::byte>(number));
        number >>= 8;
    }
    return result;
}

std::byte calculateChecksum(ByteSequence byteSequence) {
    uint64_t sum = 0;
    for (auto byte : byteSequence) {
        sum += std::to_integer<uint64_t>(byte);
    }
    return std::byte(256 - sum % 256);
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

ByteSequence makeRecord(const std::string& str) {
    ByteSequence strEncoded = encodeString(str);
    ByteSequence strLength = encodeNumberLittleEndian(strEncoded.size(), 1);

    ByteSequence record = strLength;
    record.insert(record.end(), strEncoded.begin(), strEncoded.end());

    return record;
}