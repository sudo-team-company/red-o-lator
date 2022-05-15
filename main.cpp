#include <iostream>
#include <memory>

void ala(size_t& a) {
    a = 3;
}
int main() {
    auto a = std::byte(1);
    auto b = uint8_t(a);
    std::cout << int(a) << " " << b << " " << int(b) << std::endl;

//    size_t size = 20;
//    std::unique_ptr<uint8_t[]> bytes = std::make_unique<uint8_t[]>(size);
//    for (size_t i = 0; i < size; ++i) {
//        bytes[i] = 0;
//    }
//    float value = -2;
//    auto valueAsUll = static_cast<uint32_t>(value);
//    memcpy(&valueAsUll, (unsigned char*) (&value), sizeof(value));
//    std::cout << "Value (ull): " << std::bitset<32>(valueAsUll) << std::endl;
//    std::cout << "Value: " << std::bitset<32>(value) << std::endl;
//    auto valueAddr = 4; //=> bytes[7]
//
//    memcpy(bytes.get() + valueAddr,&valueAsUll, sizeof(value));
//
//    std::cout << "Start:  " << sizeof(value) << std::endl;
//    for (size_t i = 0; i < size; ++i) {
//        std::cout << std::bitset<8>(bytes[i]) << " ";
//    }
}