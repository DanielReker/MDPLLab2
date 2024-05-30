#pragma once

#include <string>

#include "LineData.h"

std::string formatListingLine(const LineData& data) {
    std::string offsetStr;
    if (data.bytes.size() > 0) {
        std::stringstream offsetStrStream;
        offsetStrStream << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << data.offset << ':';
        offsetStr = offsetStrStream.str();
    } else offsetStr = std::string(9, ' ');
    return std::format("{:6}  {}  {:24} | {}\n", data.lineNumber, offsetStr, toString(data.bytes), data.line);
}