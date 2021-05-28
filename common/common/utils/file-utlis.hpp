#pragma once

#include <fstream>
#include <iostream>
#include <vector>

namespace utils {
static std::vector<unsigned char> readBinaryFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),
                                      {});
}
}  // namespace utils
