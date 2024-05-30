#pragma once

#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <concepts>

#include "ByteSequence.h"

std::string toLower(const std::string& str) {
    std::string result;
    std::transform(str.begin(), str.end(), std::back_inserter(result),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

struct ParsedLine {
    std::string label;
    std::string mnemonic;
    std::vector<std::string> arguments;
    std::string comment;
};

ParsedLine parseLine(const std::string& line) {
    static const std::regex linePattern(R"(^\s*(?:(\w+):)?\s*(\w+)?\s*([^;]*)?(?:;\s*(.*))?$)");
    std::smatch lineMatch;
    if (std::regex_match(line, lineMatch, linePattern)) {
        ParsedLine result;
        result.label = toLower(lineMatch[1]);
        result.mnemonic = toLower(lineMatch[2]);
        result.comment = toLower(lineMatch[4]);

        std::stringstream argumentsStream(toLower(lineMatch[3]));
        std::string argument;
        while (std::getline(argumentsStream, argument, ',')) {
            result.arguments.push_back(argument);
        }

        return result;
    } else throw std::invalid_argument("Invalid syntax");
}

std::string removeWhitespace(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) { return std::isspace(c); }), str.end());
    return str;
}

inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
}

ByteSequence parseStringLiteral(std::string stringLiteral) {
    auto firstQuote = std::find_if(stringLiteral.begin(), stringLiteral.end(), isQuote);
    if (firstQuote == stringLiteral.end())
        throw std::invalid_argument("Not correct string literal");

    stringLiteral.erase(stringLiteral.begin(), firstQuote + 1);

    std::reverse(stringLiteral.begin(), stringLiteral.end());
    
    auto lastQuote = std::find_if(stringLiteral.begin(), stringLiteral.end(), isQuote);
    stringLiteral.erase(stringLiteral.begin(), lastQuote + 1);

    std::reverse(stringLiteral.begin(), stringLiteral.end());

    ByteSequence result;
    std::transform(stringLiteral.begin(), stringLiteral.end(), std::back_inserter(result), [](char c) { return std::byte(c); });
    return result;
}

template<std::integral T>
T parseNumber(std::string numberStr) {
    numberStr = removeWhitespace(numberStr);

    bool invert = false;
    if (numberStr[0] == '-') {
        invert = true;
        numberStr.erase(numberStr.begin()); // Remove minus
    }

    numberStr = toLower(numberStr);

    int radix = 10; // Numbers are decimals by default
    
    // Check for octal (0), binary (0b) and hexadecimal (0x) prefix
    if (numberStr.size() > 1 && numberStr[0] == '0') {
        int prefixSize = 1;
        if (numberStr[1] == 'b') {
            radix = 2;
            prefixSize++;
        } else if (numberStr[1] == 'x') {
            radix = 16;
            prefixSize++;
        } else radix = 8;

        numberStr.erase(numberStr.begin(), numberStr.begin() + prefixSize); // Remove prefix
    }

    T result = static_cast<T>(std::stoll(numberStr, nullptr, radix));
    if (invert) result = -result;
    return result;
}