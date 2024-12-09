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
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return clientSocket;
}

void connectToServer(SOCKET clientSocket, const std::string& serverAddress, int port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());

    std::cout << "Attempting to connect to " << serverAddress << ":" << port << std::endl;

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server: " << serverAddress << ":" << port << std::endl;
}

int communicateWithLoadBalancer(SOCKET clientSocket) {
    // Send a message to the load balancer
    std::string message = "Hello from client!";
    send(clientSocket, message.c_str(), message.length(), 0);

    // Receive response from the load balancer
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Load balancer response: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to receive response from load balancer." << std::endl;
    }

    closesocket(clientSocket);

    int portserver = 0;
    std::string bufferStr(buffer);
    size_t colonPos = bufferStr.find(':');
    if (colonPos != std::string::npos) {
        std::string ip = bufferStr.substr(0, colonPos);
        portserver = std::stoi(bufferStr.substr(colonPos + 1));  // Converte a parte da porta para int
    } else {
        std::cerr << "Formato inválido" << std::endl;
    }

    return portserver;
}

void communicateWithServer(SOCKET clientSocket) {
    // Send a message to the server
    std::string message = "Hello from client!";
    send(clientSocket, message.c_str(), message.length(), 0);

    // Receive response from the server
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Server response: " << buffer << std::endl;
    } else {
        std::cerr << "Failed to receive response from server." << std::endl;
    }

    closesocket(clientSocket);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: client.exe <LOAD_BALANCER_IP> <PORT>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string loadBalancerIP = argv[1];
    int port = std::stoi(argv[2]);

    initializeWinsock();

    // Create socket
    SOCKET clientSocket = createSocket();

    // Connect to load balancer
    connectToServer(clientSocket, loadBalancerIP, port);

    // Communicate with load balancer
    int portserver = communicateWithLoadBalancer(clientSocket);

    // Create socket
    clientSocket = createSocket();

    // Connect to server
    connectToServer(clientSocket, loadBalancerIP, portserver);

    // Communicate with server
    communicateWithServer(clientSocket);

    WSACleanup();
    return 0;
}