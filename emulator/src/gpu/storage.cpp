//
// Created by Diana Kudaiberdieva
//

#include "storage.h"
#include <string>
std::vector<uint8_t> Storage::read_data(uint64_t address,
                                        uint32_t offset,
                                        uint32_t byteSize) {
    auto result = std::vector<uint8_t>();
    uint64_t addr = address + offset;
    for (uint64_t curAddr = addr; curAddr < addr + byteSize; ++curAddr) {
        validate_addr(curAddr);
        result.push_back(data_[curAddr]);
    }
    return result;
}
uint32_t Storage::read_4_bytes(uint64_t address, uint32_t offset) {
    auto result = read_data(address, offset, 4);
    return static_cast<uint32_t>(result[0]) << 24 |
           static_cast<uint32_t>(result[1]) << 16 |
           static_cast<uint32_t>(result[2]) << 8  | static_cast<uint32_t>(result[3]);
}

void Storage::write_data(uint64_t addr, uint32_t offset, uint16_t value) {
    data_[addr + offset] = static_cast<uint8_t>(value >> 8);
    data_[addr + offset + 1] = static_cast<uint8_t>(value);
}
void Storage::write_data(uint64_t addr, uint32_t offset, uint32_t value) {
    write_data(addr, offset, static_cast<uint16_t>(value >> 16));
    write_data(addr, offset + 2, static_cast<uint16_t>(value));
}
void Storage::write_data(uint64_t addr, uint32_t offset, uint64_t value) {
    write_data(addr, offset, static_cast<uint32_t>(value >> 32));
    write_data(addr, offset + 4, static_cast<uint32_t>(value));
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
