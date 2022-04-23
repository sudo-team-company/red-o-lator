#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

namespace filesystem = std::filesystem;

namespace utils {
static std::vector<unsigned char> readBinaryFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);

    if (!input.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),
                                      {});
}

static void writeFileBase(const std::string& path,
                          std::ios::openmode mode,
                          const std::function<void(std::ofstream&)>& work) {
    const auto parent = filesystem::path(path).parent_path();
    filesystem::create_directories(parent);

    std::ofstream output(path, mode);

    if (!output.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    try {
        work(output);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    output.close();
}

static void writeFile(const std::string& path, const std::string& text) {
    writeFileBase(path, std::ios::out, [&](auto& output) {
        output << text;
    });
}

static void writeBinaryFile(const std::string& path,
                            const std::byte* bytes,
                            const size_t size) {
    writeFileBase(path, std::ios::binary, [&](auto& output) {
        output.write(reinterpret_cast<const char*>(bytes), size);
    });
}

static bool fileExists(const std::string& path) {
    return filesystem::exists(path);
}
}  // namespace utils
