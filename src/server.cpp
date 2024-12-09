#include <iostream>
#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

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

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received message from client: " << buffer << std::endl;

        // Respond to the client
        std::string response = "Hello from server!";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    closesocket(clientSocket);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: server.exe <PORT>" << std::endl;
        return EXIT_FAILURE;
    }

    int port = std::stoi(argv[1]);

    initializeWinsock();

    // Create socket
    SOCKET serverSocket = createSocket();

    // Configure server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind socket
    bindSocket(serverSocket, serverAddress);

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running on port " << port << "..." << std::endl;

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

        std::cout << "Client connected." << std::endl;

        // Handle client connection
        handleClient(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}