//
// Created by Diana Kudaiberdieva
//

#include "storage.h"
std::vector<uint8_t> Storage::read_data(uint64_t address, uint32_t offset, uint32_t byteSize) {
    auto result = std::vector<uint8_t>();
    uint64_t addr = address + offset;
    for (uint64_t curAddr = addr; curAddr < addr + byteSize; ++curAddr) {
        validate_addr(curAddr);
        result.push_back(data[curAddr]);
    }
    return result;
}

void Storage::validate_addr(uint64_t addr) {
    if (addr < 0 || addr >= data.size()) {
        throw std::runtime_error(&"Invalid addr in storage: " [addr]);
    }
}

Storage* Storage::storage_ = nullptr;

Storage* Storage::get_instance() {
    if (storage_ == nullptr) {
        storage_ = new Storage();
    }
    return storage_;
}
