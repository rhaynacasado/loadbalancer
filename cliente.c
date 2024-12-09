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

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Erro ao inicializar Winsock");
        exit(1);
    }
#endif

    int sock;
    struct sockaddr_in server_addr;
    char message[100], response[100];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Substituição de inet_pton

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro ao conectar ao servidor");
        exit(1);
    }

    printf("Conectado ao servidor. Envie mensagens:\n");
    while (1) {
        printf("Mensagem: ");
        fgets(message, sizeof(message), stdin);
        send(sock, message, strlen(message), 0);

        if (recv(sock, response, sizeof(response), 0) > 0) {
            printf("Resposta do servidor: %s\n", response);
        }
    }

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif
    return 0;
}
