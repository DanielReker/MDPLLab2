#pragma once

#include <map>
#include <string>

#include "ByteSequence.h"

std::map<std::string, ByteSequence> rel16Opcodes{
    { "call",  { std::byte(0xE8) } },

    { "jb",  { std::byte(0x0F), std::byte(0x82) } },
    { "jnae",  { std::byte(0x0F), std::byte(0x82) } },
    { "jc",  { std::byte(0x0F), std::byte(0x82) } },

    { "jbe",  { std::byte(0x0F), std::byte(0x86) } },
    { "jna",  { std::byte(0x0F), std::byte(0x86) } },

    { "jl",  { std::byte(0x0F), std::byte(0x8C) } },
    { "jnge",  { std::byte(0x0F), std::byte(0x8C) } },

    { "jle",  { std::byte(0x0F), std::byte(0x8E) } },
    { "jng",  { std::byte(0x0F), std::byte(0x8E) } },

    { "jmp", { std::byte(0xE9) } },

    { "jnb",  { std::byte(0x0F), std::byte(0x83) } },
    { "jae",  { std::byte(0x0F), std::byte(0x83) } },
    { "jnc",  { std::byte(0x0F), std::byte(0x83) } },

    { "jnbe",  { std::byte(0x0F), std::byte(0x87) } },
    { "ja",  { std::byte(0x0F), std::byte(0x87) } },

    { "jnl",  { std::byte(0x0F), std::byte(0x8D) } },
    { "jge",  { std::byte(0x0F), std::byte(0x8D) } },

    { "jnle",  { std::byte(0x0F), std::byte(0x8F) } },
    { "jg",  { std::byte(0x0F), std::byte(0x8F) } },

    { "jno",  { std::byte(0x0F), std::byte(0x81) } },

    { "jnp",  { std::byte(0x0F), std::byte(0x8B) } },
    { "jpo",  { std::byte(0x0F), std::byte(0x8B) } },

    { "jns",  { std::byte(0x0F), std::byte(0x89) } },

    { "jnz",  { std::byte(0x0F), std::byte(0x85) } },
    { "jne",  { std::byte(0x0F), std::byte(0x85) } },

    { "jo",  { std::byte(0x0F), std::byte(0x80) } },

    { "jp",  { std::byte(0x0F), std::byte(0x8A) } },
    { "jpe",  { std::byte(0x0F), std::byte(0x8A) } },

    { "jz",  { std::byte(0x0F), std::byte(0x84) } },
    { "je",  { std::byte(0x0F), std::byte(0x84) } },
};