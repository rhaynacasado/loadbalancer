#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#define PORT 9000
#define NUM_SERVERS 2

const char *server_ips[] = {"127.0.0.1", "127.0.0.1"};
const int server_ports[] = {9001, 9002};
int current_server = 0;

int connect_to_server(const char *ip, int port) {
    int sock;
    struct sockaddr_in server_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket para servidor");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);  // Substituição de inet_pton

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar ao servidor");
#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return -1;
    }
    return sock;
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Erro ao inicializar Winsock");
        exit(1);
    }
#endif

    int listener, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];
    int server_sock;

    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listener, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao associar o socket");
        exit(1);
    }

    listen(listener, 5);
    printf("Balanceador aguardando conexões...\n");

    while (1) {
        client_sock = accept(listener, (struct sockaddr*)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        server_sock = connect_to_server(server_ips[current_server], server_ports[current_server]);
        current_server = (current_server + 1) % NUM_SERVERS;

        if (server_sock < 0) {
#ifdef _WIN32
            closesocket(client_sock);
#else
            close(client_sock);
#endif
            continue;
        }

        int bytes;
        while ((bytes = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
            send(server_sock, buffer, bytes, 0);
            bytes = recv(server_sock, buffer, sizeof(buffer), 0);
            send(client_sock, buffer, bytes, 0);
        }

#ifdef _WIN32
        closesocket(server_sock);
        closesocket(client_sock);
#else
        close(server_sock);
        close(client_sock);
#endif
    }

#ifdef _WIN32
    closesocket(listener);
    WSACleanup();
#else
    close(listener);
#endif

    return 0;
}
