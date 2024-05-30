#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstdint>

#include "ByteSequence.h"
#include "Rel16Opcodes.h"
#include "Parsing.h"
#include "LineData.h"
#include "Listing.h"
#include "DefineByteDirectives.h"


std::vector<std::unique_ptr<LineData>> program;

std::map<std::string, LineData*> labelToLineData;

int64_t calculateRel16InstructionLen(const std::string& mnemonic) {
    // Opcode + two bytes of rel16 operand
    return static_cast<int64_t>(rel16Opcodes[mnemonic].size()) + 2;
}

std::map<std::string, std::vector<LineData*>> toReencode;

ByteSequence encodeRel16Instruction(const std::string& mnemonic, int64_t offset) {
    ByteSequence encodedInstruction = rel16Opcodes[mnemonic]; // Get proper opcode

    // Add rel16 operand (2-byte offset)
    ByteSequence encodedOffset = encodeNumberLittleEndian<int64_t>(offset, 2);
    encodedInstruction.insert(encodedInstruction.end(), encodedOffset.begin(), encodedOffset.end());

    return encodedInstruction;
}

ByteSequence encodeRel16Instruction(LineData& from, std::string& labelTo) {
    auto instructionSize = calculateRel16InstructionLen(from.parsed.mnemonic);
    if (labelToLineData.contains(labelTo)) {
        // Jumps are done from end of current instruction (offset of "from" + length of instruction)
        // to begin of other instruction (offset of "to")
        return encodeRel16Instruction(
            from.parsed.mnemonic,
            labelToLineData[labelTo]->offset - (from.offset + instructionSize)
        );
    } else { // Unknown labelTo (probably forward reference)
        toReencode[labelTo].push_back(&from); // Mark to reencode later
        return ByteSequence(instructionSize, std::byte(0x00)); // Dummy with proper size
    }
}


int main() {
    std::ifstream input("main.asm");
    std::ofstream output("main.com", std::ios::binary);
    std::ofstream listing("main.lst");

    // Parse file line-by-line
    std::string line;
    int currentLineNumber = 1;
    int64_t offset = 0;
    while (std::getline(input, line)) {
        ParsedLine parsedLine;
        try {
            parsedLine = parseLine(line);
        } catch (const std::exception& exception) {
            std::cout << std::format("Error on line {}: {}", currentLineNumber, exception.what());
            return 0;
        }

        // Process parsed line data
        program.push_back(std::make_unique<LineData>());
        LineData& lineData = *program.back();
        lineData.line = line;
        lineData.lineNumber = currentLineNumber;
        lineData.offset = offset;
        lineData.parsed = parsedLine;
        labelToLineData[lineData.parsed.label] = &lineData;

        if (rel16Opcodes.contains(parsedLine.mnemonic)) { // Different jumps and call
            std::string argument = removeWhitespace(parsedLine.arguments[0]);

            if (argument[0] == '$') { // Offset from current instruction offset
                argument.erase(argument.begin()); // Remove dollar sign
                auto instructionOffset = parseNumber<int64_t>(argument) - calculateRel16InstructionLen(parsedLine.mnemonic);
                lineData.bytes = encodeRel16Instruction(parsedLine.mnemonic, instructionOffset);
            } else { // To label
                lineData.bytes = encodeRel16Instruction(lineData, argument);
            }
        } else if (parsedLine.mnemonic == "org") { // org directive
            offset = parseNumber<int64_t>(parsedLine.arguments[0]);
        } else if (defineByteDirectiveSize.contains(parsedLine.mnemonic)) { // db, dw, dd, ...
            // 1 for db, 2 for dw, etc
            size_t size = defineByteDirectiveSize[parsedLine.mnemonic];
            for (auto argument : parsedLine.arguments) {
                ByteSequence bytes;
                try {
                    bytes = parseStringLiteral(argument);
                    // Alignment
                    size_t alignedSize = bytes.size();
                    if (alignedSize % size != 0) {
                        alignedSize = size * (alignedSize / size + 1);
                    }
                    bytes.resize(alignedSize);
                } catch (const std::exception&) {
                    bytes = encodeNumberLittleEndian<int64_t>(parseNumber<int64_t>(argument), static_cast<int>(size));
                }
                // Add parsed argument bytes to output
                lineData.bytes.insert(lineData.bytes.end(), bytes.begin(), bytes.end());
            }
        }

        if (toReencode.contains(lineData.parsed.label)) {
            for (auto lineDataPtr : toReencode[lineData.parsed.label]) {
                lineDataPtr->bytes = encodeRel16Instruction(*lineDataPtr, lineData.parsed.label);
            }
        }

        // For next line
        offset += lineData.bytes.size();
        currentLineNumber++;
    }

    // Form result binary file and listing
    for (const auto& lineData : program) {
        // Binary output
        output.write(reinterpret_cast<const char*>(lineData->bytes.data()), lineData->bytes.size());

        // Listing output
        listing << formatListingLine(*lineData);
    }
}
