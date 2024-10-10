#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <sys/uio.h>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include "Config.hpp"
#include "Features.hpp"

namespace fs = std::filesystem;

namespace Memory {

pid_t GetPID() {
    if (PID > 0)
        return PID;

    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory())
            continue;

        std::ifstream commFile(entry.path() / "comm");
        std::string commName;
        std::getline(commFile, commName);

        if (commName == "r5apex.exe") {
            PID = std::stoi(entry.path().filename());
            break;
        }
    }
    return PID;
}

bool IsValidPointer(uintptr_t Pointer) {
    return Pointer > 0x00010000 && Pointer < 0x7FFFFFFEFFFF;
}

template <typename T>
bool Read(uintptr_t address, T& outBuffer) {
    if (!IsValidPointer(address))
        return false;

    pid_t pid = GetPID();
    struct iovec local_iov = { &outBuffer, sizeof(T) };
    struct iovec remote_iov = { reinterpret_cast<void*>(address), sizeof(T) };
    
    ssize_t bytesRead = process_vm_readv(pid, &local_iov, 1, &remote_iov, 1, 0);
    return bytesRead == sizeof(T);
}

std::string ReadString(uintptr_t address, size_t size) {
    std::vector<char> buffer(size);
    if (!Read(address, buffer[0])) {
        throw std::runtime_error("Failed to read string at address: " + std::to_string(address));
    }
    return std::string(buffer.data(), size);
}

std::string ConvertPointerToHexString(uintptr_t pointer) {
    std::stringstream stream;
    stream << "0x" << std::hex << pointer;
    return stream.str();
}
}
