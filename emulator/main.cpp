#include <iostream>
#include "runtime/runtime-impl.cpp"

int main() {
    std::cout << helloFromDriver();
    std::cout << "Hello from emulator";

    return 0;
}
