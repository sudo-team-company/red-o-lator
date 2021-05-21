//
// Created by Diana Kudaiberdieva
//

#include "storage.h"
#include <string>
std::vector<uint8_t> Storage::read_data(uint64_t address, uint32_t offset, uint32_t byteSize) {
    auto result = std::vector<uint8_t>();
    uint64_t addr = address + offset;
    for (uint64_t curAddr = addr; curAddr < addr + byteSize; ++curAddr) {
        validate_addr(curAddr);
        result.push_back(data_[curAddr]);
    }
    return result;
}

void Storage::write_data(uint64_t addr, uint32_t offset, uint32_t value) {
    for (size_t i = 0; i < 4; ++i) {
        data_[addr + offset + i] = static_cast<uint8_t>(value >> 8 * (3 - i));
    }
}

void Storage::validate_addr(uint64_t addr) {
    if (addr < 0 || addr >= data_.size()) {
        throw std::runtime_error("Invalid addr in storage: " + std::to_string(addr));
    }
}

Storage* Storage::storage_ = nullptr;

Storage* Storage::get_instance() {
    if (storage_ == nullptr) {
        storage_ = new Storage();
    }
    return storage_;
}

void Storage::init(size_t bufferSize) {
    data_.resize(bufferSize);
}
