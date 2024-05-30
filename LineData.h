#pragma once

#include <string>
#include <cstdint>

#include "ByteSequence.h"
#include "Parsing.h"


struct LineData {
    std::string line;
    int lineNumber = -1;
    ParsedLine parsed;
    int64_t offset = 0;
    ByteSequence bytes;
};