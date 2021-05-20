//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_STORAGE_H
#define RED_O_LATOR_STORAGE_H

#include <vector>
#include <cstdint>

struct Storage {
    std::vector<uint8_t> data;

    ~Storage() { delete storage_; }
    Storage(Storage& other) = delete;
    void operator=(const Storage& other) = delete;
    static Storage* get_instance();

    std::vector<uint8_t> read_data(uint64_t, uint32_t, uint32_t);

   private:
    Storage() {
        data = std::vector<uint8_t>();
    }

    static Storage* storage_;

    void validate_addr(uint64_t);
};

#endif  // RED_O_LATOR_STORAGE_H
