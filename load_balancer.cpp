#include <iostream>
#include <winsock2.h>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed. Error: " << WSAGetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

SOCKET createSocket() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return serverSocket;
}

void bindSocket(SOCKET serverSocket, sockaddr_in &address) {
    if (bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void distributeRequest(SOCKET clientSocket, const std::vector<std::string> &serverList, int &roundRobinIndex) {
    // Select the server using Round Robin
    std::string selectedServer = serverList[roundRobinIndex];
    roundRobinIndex = (roundRobinIndex + 1) % serverList.size();

    std::cout << "Forwarding request to server: " << selectedServer << std::endl;

    // Send the server's address to the client
    send(clientSocket, selectedServer.c_str(), selectedServer.length(), 0);
    closesocket(clientSocket);
}

int main() {
    initializeWinsock();

    // Create socket
    SOCKET serverSocket = createSocket();

    // Configure server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket
    bindSocket(serverSocket, serverAddress);

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Load Balancer is running on port " << PORT << "..." << std::endl;

    // List of servers
    std::vector<std::string> serverList = {"192.168.1.101:8081", "192.168.1.102:8082", "192.168.1.103:8083"};
    int roundRobinIndex = 0;

    while (true) {
        // Accept incoming client connections
        SOCKET clientSocket;
        sockaddr_in clientAddress;
        int clientAddressLen = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed. Error: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "Received connection from client." << std::endl;

        // Distribute request
        distributeRequest(clientSocket, serverList, roundRobinIndex);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
