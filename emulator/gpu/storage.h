//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_STORAGE_H
#define RED_O_LATOR_STORAGE_H

#include <vector>
#include <cstdint>

struct Storage {

    ~Storage() { delete storage_; }
    Storage(Storage& other) = delete;
    void operator=(const Storage& other) = delete;
    static Storage* get_instance();

    void init(size_t bufferSize);
    std::vector<uint8_t> read_data(uint64_t, uint32_t, uint32_t);
    void write_data(uint64_t, uint32_t, uint32_t);

   private:
    std::vector<uint8_t> data_;
    Storage() {
        data_ = std::vector<uint8_t>();
    }

    static Storage* storage_;

    void validate_addr(uint64_t);
};

#endif  // RED_O_LATOR_STORAGE_H
