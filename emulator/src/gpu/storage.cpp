#include "storage.h"
#include <string>

std::vector<uint8_t> Storage::read_data(uint64_t address,
                                        uint32_t offset,
                                        uint32_t byteSize) {
    auto result = std::vector<uint8_t>();
    uint64_t addr = address + offset;
    for (uint64_t curAddr = addr; curAddr < addr + byteSize; ++curAddr) {
        validate_addr(curAddr);
        result.push_back(_data[curAddr]);
    }
    return std::move(result);
}

uint32_t Storage::read_4_bytes(uint64_t address, uint32_t offset) {
    auto result = read_data(address, offset, 4);
    return static_cast<uint32_t>(result[0]) << 24 | static_cast<uint32_t>(result[1]) << 16 |
           static_cast<uint32_t>(result[2]) << 8  | static_cast<uint32_t>(result[3]);
}

uint64_t Storage::read_8_bytes(uint64_t address) {
    auto result = read_data(address, 0, 8);
    return static_cast<uint64_t>(result[0]) << 56 | static_cast<uint64_t>(result[1]) << 48 |
           static_cast<uint64_t>(result[2]) << 40 | static_cast<uint64_t>(result[3]) << 32 |
           static_cast<uint64_t>(result[4]) << 24 | static_cast<uint64_t>(result[5]) << 16 |
           static_cast<uint64_t>(result[6]) << 8  | static_cast<uint64_t>(result[7]);
}

void Storage::write_data(uint64_t addr, uint32_t offset, uint8_t value) {
    validate_addr(addr + offset);
    _data[addr + offset] = value;
}

void Storage::write_data(uint64_t addr, uint32_t offset, uint16_t value) {
    write_data(addr, offset, uint8_t (value >> 8));
    write_data(addr, offset + 1, uint8_t (value));
}
void Storage::write_data(uint64_t addr, uint32_t offset, uint32_t value) {
    write_data(addr, offset, static_cast<uint16_t>(value >> 16));
    write_data(addr, offset + 2, static_cast<uint16_t>(value));
}
void Storage::write_data(uint64_t addr, uint32_t offset, uint64_t value) {
    write_data(addr, offset, static_cast<uint32_t>(value >> 32));
    write_data(addr, offset + 4, static_cast<uint32_t>(value));
}

void Storage::write_data(uint64_t addr, uint32_t offset, float value) {
    write_data(addr, offset, static_cast<uint32_t>(value));
}

void Storage::validate_addr(uint64_t addr) {
    if (addr < 0 || addr >= _size) {
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
    _size = bufferSize;
    _data = std::make_unique<uint8_t[]>(bufferSize);
}
