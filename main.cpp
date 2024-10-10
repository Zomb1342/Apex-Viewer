#include <iostream>
#include <string>
#include <stdexcept>
#include "memory.hpp"

using namespace Memory;

template <typename T>
void readMemory(uintptr_t address) {
    T value;
    if (Read(address, value)) {
        std::cout << value << std::endl;
    } else {
        std::cout << "Failed to read memory at address: " << ConvertPointerToHexString(address) << std::endl;
    }
}

int main() {
    std::string inputAddress, inputType;
    int size;

    // Prompt user for input
    std::cout << "Enter memory address: ";
    std::cin >> inputAddress;
    std::cout << "Enter variable type (integer, float, string): ";
    std::cin >> inputType;
    std::cout << "Enter number of bytes to read: ";
    std::cin >> size;

    // Convert string address to uintptr_t
    uintptr_t address = std::stoul(inputAddress, nullptr, 16);

    try {
        if (inputType == "integer") {
            if (size != 4) {
                std::cout << "Invalid size for integer. Expected 4 bytes." << std::endl;
                return 1;
            }
            readMemory<int>(address);
        } else if (inputType == "float") {
            if (size != 4) {
                std::cout << "Invalid size for float. Expected 4 bytes." << std::endl;
                return 1;
            }
            readMemory<float>(address);
        } else if (inputType == "string") {
            std::string value = ReadString(address, size);
            std::cout << value << std::endl;
        } else {
            std::cout << "Unsupported variable type." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
