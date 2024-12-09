#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
    std::cout << "Winsock initialized successfully!" << std::endl;
    WSACleanup();
    return 0;
}
