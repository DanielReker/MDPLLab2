#pragma once

#include <map>
#include <string>

std::map<std::string, size_t> defineByteDirectiveSize {
    { "db", 1 },
    { "dw", 2 },
    { "dd", 4 },
    { "dq", 8 }
};