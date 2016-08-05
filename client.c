#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_PORT 8888
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024


int main(int argc , char *argv[])
{
    int server_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] , server_reply[BUFFER_SIZE];

    server_socket = socket(AF_INET , SOCK_STREAM , 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (connect(server_socket , (struct sockaddr *)&server_addr , sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s:%d...\n", SERVER_ADDRESS, SERVER_PORT);

    while(1)
    {
        memset(server_reply, 0, BUFFER_SIZE);

        printf("Enter message : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        printf("message : %s", buffer);

        if( send(server_socket , buffer , strlen(buffer) , 0) < 0) {
            perror("send");
            continue;
        }

        if( recv(server_socket , server_reply , BUFFER_SIZE , 0) < 0) {
            perror("recv");
            break;
        }

        printf("received : %s\n", server_reply);

    }

    close(server_socket);
    return 0;
}
