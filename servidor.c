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

#define PORT 9001 // Altere para 9002 no segundo servidor

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

    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket");
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
    printf("Servidor rodando na porta %d...\n", PORT);

    while (1) {
        client_sock = accept(listener, (struct sockaddr*)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        int bytes;
        while ((bytes = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
            buffer[bytes] = '\0';
            printf("Recebido: %s", buffer);
            snprintf(buffer, sizeof(buffer), "Processado pelo servidor na porta %d\n", PORT);
            send(client_sock, buffer, strlen(buffer), 0);
        }

#ifdef _WIN32
        closesocket(client_sock);
#else
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
