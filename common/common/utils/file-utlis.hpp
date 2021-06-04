#pragma once

#include <cstddef>
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

static void writeFile(const std::string& path, const std::string& text) {
    std::ofstream output(path, std::ios::binary);

    if (!output.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    output << text;

    output.close();
}

static void writeBinaryFile(const std::string& path,
                            const std::byte* bytes,
                            const size_t size) {
    std::ofstream output(path, std::ios::binary);

    if (!output.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    output.write(reinterpret_cast<const char*>(bytes), size);

    output.close();
}

static bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}
}  // namespace utils
